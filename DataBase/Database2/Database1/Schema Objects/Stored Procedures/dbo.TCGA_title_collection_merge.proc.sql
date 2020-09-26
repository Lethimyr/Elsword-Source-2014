CREATE PROCEDURE [dbo].[TCGA_title_collection_merge]
	@iLoginUID_ [int],
	@iMainMissionID_ [int],
	@iSubMissionID_ [int],
	@iTCCount_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081117. microcat.

{ call dbo.TCGA_title_collection_merge ( %d, %d, %d, %d ) }

{ call dbo.TCGA_title_collection_merge ( @1, @2, @3, @4 ) }
@1 ; LoginUID int
@2 ; MainMissionID int
@3 ; SubMissionID int
@4 ; TCCount int

1 return ( @1 )
@1 ; OK int

OK
0        ; 성공
-1       ; 유저 정보가 존재하지 않음
-2       ; 타이틀 획득 미션이 진행중이 아님
-101이하 ; 트랜잭션 에러
*/

if not exists
(
    select *
    from dbo.Users
    where
        LoginUID = @iLoginUID_
)
begin    select @iOK = -1    goto end_proc    end

if not exists
(
    select *
    from dbo.TCGTitleCollectionMain
    where
        LoginUID = @iLoginUID_
    and MainMissionID = @iMainMissionID_
    and Status = 1
)
begin    select @iOK = -2    goto end_proc    end

if exists
(
    select *
    from dbo.TCGTItleCollectionSub
    where
        LoginUID = @iLoginUID_
    and MainMissionID = @iMainMissionID_
    and SubMissionID = @iSubMissionID_
)
begin
    begin transaction

    update a
    with (updlock)
    set
        a.TCCount = @iTCCount_
    from dbo.TCGTitleCollectionSub
    as a
    with (updlock)
    where
        LoginUID = @iLoginUID_
    and MainMissionID = @iMainMissionID_
    and SubMissionID = @iSubMissionID_
    option (maxdop 1)

    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end
end
else
begin
    begin transaction

    insert into dbo.TCGTitleCollectionSub
    with (updlock)
    (
        LoginUID
    ,   MainMissionID
    ,   SubMissionID
    ,   TCCount
    )
    select
        @iLoginUID_
    ,   @iMainMissionID_
    ,   @iSubMissionID_
    ,   @iTCCount_
    option (maxdop 1)

    if @@error <> 0
    begin    select @iOK = -102    goto fail_tran    end
end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
select
    @iOK


