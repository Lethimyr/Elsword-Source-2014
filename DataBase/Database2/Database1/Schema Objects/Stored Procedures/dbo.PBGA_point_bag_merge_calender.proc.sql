CREATE PROCEDURE [dbo].[PBGA_point_bag_merge_calender]
	@iLoginUID_ [int],
	@sdtRegDateD_ [smalldatetime],
	@iSealType_ [tinyint],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081125. microcat.

{ call dbo.PBGA_point_bag_merge_calender ( %d, N'%s', %d ) }

{ call dbo.PBGA_point_bag_merge_calender ( @1, @2, @3 ) }
@1 ; LoginUID int
@2 ; RegDateD smalldatetime
@3 ; SealType tinyint

1 return ( @1 )
@1 ; OK int
    0        ; 성공
    -1       ; 유저정보가 없음
    -2       ; 포인트정보가 없음
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
    from dbo.GPointBag
    where
        LoginUID = @iLoginUID_
)
begin    select @iOK = -2    goto end_proc    end

if exists
(
    select *
    from dbo.GPointBagCalender
    where
        LoginUID = @iLoginUID_
    and RegDate = @sdtRegDateD_
)
begin
    begin transaction

    update a
    with (updlock)
    set
        SealType = @iSealType_
    from dbo.GPointBagCalender
    as a
    with (updlock)
    where
        LoginUID = @iLoginUID_
    and RegDate = @sdtRegDateD_ 

    if @@error <> 0
    begin    select @iOK = -101    goto end_proc    end
end
else
begin
    begin transaction

    insert into dbo.GPointBagCalender
    with (updlock)
    (
        LoginUID
    ,   RegDate
    ,   SealType
    )
    select
        @iLoginUID_
    ,   @sdtRegDateD_
    ,   @iSealType_

    if @@error <> 0
    begin    select @iOK = -102    goto end_proc    end
end

goto end_proc

fail_tran:
rollback transaction

end_proc:
select
    @iOK


