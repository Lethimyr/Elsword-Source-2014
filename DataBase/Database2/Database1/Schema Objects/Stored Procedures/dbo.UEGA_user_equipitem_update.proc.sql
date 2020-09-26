CREATE PROCEDURE [dbo].[UEGA_user_equipitem_update]
	@iLoginUID_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081127. microcat.

L"{ call dbo.UEGA_user_equipitem_update ( %d ) }"

{ call dbo.UEGA_user_equipitem_update ( @1 ) }
@1 ; LoginUID int

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
    and Deleted = 0
)
begin
    begin transaction

    update a
    with (updlock)
    set
        Deleted = 1
    from dbo.UEGAUserEquipItem
    as a
    with (updlock)
    where
        LoginUID = @iLoginUID_
    and Deleted = 0

    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end

    commit transaction
end

goto end_proc

fail_tran:
rollback transaction

end_proc:
select
    @iOK


