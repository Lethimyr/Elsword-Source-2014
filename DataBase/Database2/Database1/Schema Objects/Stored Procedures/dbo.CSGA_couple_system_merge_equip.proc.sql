CREATE PROCEDURE [dbo].[CSGA_couple_system_merge_equip]
	@iLoginUID_ [int],
	@iCharType_ [smallint],
	@iType_ [tinyint],
	@iItemUID_ [int],
	@iItemID_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081027. microcat.

{ call dbo.CSGA_couple_system_merge_equip ( %d, %d, %d, %d, %d ) }

{ call dbo.CSGA_couple_system_merge_equip ( @1, @2, @3, @4, @5 ) }
@1 ; LoginUID int ; 내꺼
@2 ; CharType smallint ; -1(유저), 0부터 엘리시스...
@3 ; Type tinyint ; 장착 아이템의 위치
@4 ; ItemUID int
@5 ; ItemID int

1 return ( @1 )
@1 ; OK int

OK
0 ; 성공
-1 ; 내꺼 유저 정보가 존재하지 않음
-2 ; 내 커플 정보가 존재하지 않음
-101이하 ; 트랜잭션 에러
*/

execute dbo.AAA_login_check
    @iLoginUID_
,   @iOK output

if @iOK <> 0
begin    select @iOK = -1    goto end_proc    end

if not exists
(
    select *
    from dbo.CSGACoupleSystem
    where
        LoginUID = @iLoginUID_
)
begin    select @iOK = -2    goto end_proc    end

if exists
(
    select *
    from dbo.CSGACoupleSystemEquip
    where
        LoginUID = @iLoginUID_
    and CharType = @iCharType_
    and Type = @iType_
)
begin
    begin transaction

    update a
    with (updlock)
    set
        ItemUID = @iItemUID_
    ,   ItemID = @iItemID_
    from dbo.CSGACoupleSystemEquip
    as a
    with (updlock)
    where
        LoginUID = @iLoginUID_
    and CharType = @iCharType_
    and Type = @iType_
    option (maxdop 1)

    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end
end
else
begin
    begin transaction

    insert into dbo.CSGACoupleSystemEquip
    with (updlock)
    (
        LoginUID
    ,   CharType
    ,   Type
    ,   ItemUID
    ,   ItemID
    )
    select
        @iLoginUID_
    ,   @iCharType_
    ,   @iType_
    ,   @iItemUID_
    ,   @iItemID_
    option (maxdop 1)

    if @@error <> 0
    begin    select @iOK = -102    goto fail_tran    end
end

execute [Log].dbo.CSMO_couple_system_insert_eqiup
    @iLoginUID_
,   @iCharType_
,   @iType_
,   @iItemUID_
,   @iItemID_
,   @iOK output

if @iOK <> 0
begin    select @iOK = -103    goto fail_tran    end


commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
select
    @iOK


