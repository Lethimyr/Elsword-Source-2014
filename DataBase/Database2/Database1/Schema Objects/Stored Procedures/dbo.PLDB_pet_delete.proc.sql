CREATE PROCEDURE [dbo].[PLDB_pet_delete]
	@iPetID_ [int],
	@iPromotion_ [int],
	@iOK [int] = 0
AS
set nocount on

begin transaction
    DELETE a
    FROM dbo.GMessageName as a
    WITH (nolock)
    WHERE ItemID = @iPetID_
    and Offset = @iPromotion_
    OPTION (MAXDOP 1)

    if @@error <> 0
    begin select @iOk = -101 goto fail_tran end

    DELETE a
    FROM dbo.GPetItem as a
    WITH (nolock)
    WHERE PetID = @iPetID_
    and Promotion = @iPromotion_
    OPTION (MAXDOP 1)

    if @@error <> 0
    begin select @iOk = -102 goto fail_tran end
commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
select @iOK as OK


