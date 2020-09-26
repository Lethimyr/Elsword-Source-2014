CREATE PROCEDURE [dbo].[UEGA_user_equipitem_merge]
	@iLoginUID_ [int],
	@iItemOrderNo_ [tinyint],
	@iItemUID_ [int],
	@iItemID_ [int],
	@iItemType_ [bit],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081127. microcat.

L"{ call dbo.UEGA_user_equipitem_merge ( %d, %d, %d, %d, %d ) }"

{ call dbo.UEGA_user_equipitem_merge ( @1, @2, @3, @4, @5 ) }
@1 ; LoginUID int
@2 ; ItemOrderNo tinyint
@3 ; ItemUID int
@4 ; ItemID int
@5 ; ItemType bit

1 return ( @1 )
@1 ; OK int
    0        ; 성공
    -101이하 ; 트랜잭션 에러
*/

if exists
(
    select *
    from dbo.UEGAUserEquipItem
    where
        LoginUID = @iLoginUID_
    and ItemOrderNo = @iItemOrderNo_
)
begin
    begin transaction

    update a
    with (updlock)
    set
        ItemUID = @iItemUID_
    ,   ItemID = @iItemID_
    ,   ItemType = @iItemType_
    ,   Deleted = 0
    from dbo.UEGAUserEquipItem
    as a
    with (updlock)
    where
        LoginUID = @iLoginUID_
    and ItemOrderNo = @iItemOrderNo_

    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end
end
else
begin
    begin transaction

    insert into dbo.UEGAUserEquipItem
    with (updlock)
    (
        LoginUID
    ,   ItemOrderNo
    ,   ItemUID
    ,   ItemID
    ,   ItemType
    ,   Deleted
    )
    select
        @iLoginUID_
    ,   @iItemOrderNo_
    ,   @iItemUID_
    ,   @iItemID_
    ,   @iItemType_
    ,   0

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


