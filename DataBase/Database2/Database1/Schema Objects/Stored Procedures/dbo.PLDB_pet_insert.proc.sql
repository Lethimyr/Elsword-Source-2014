CREATE PROCEDURE [dbo].[PLDB_pet_insert]
	@iPetID_ [int],
	@iPromotion_ [tinyint],
	@strPetName_ [nvarchar](100),
	@iPetItemID_ [int],
	@iFactor_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

if not exists
(
    SELECT *
    FROM dbo.GoodsInfoList
    WHERE GoodsID = @iPetID_
    and kind = 50
)
begin select @iOK = -1 goto end_proc end

if not exists
(
    SELECT *
    FROM dbo.GoodsInfoList
    WHERE GoodsID = @iPetItemID_
    and kind = 53
    and ItemType = 1
)
begin select @iOK = -2 goto end_proc end

if exists
(
    SELECT *
	FROM dbo.GMessageName
	WHERE ItemID = @iPetID_
	and Offset = @iPromotion_
)
begin select @iOK = -3 goto end_proc end

if exists
(
    SELECT *
	FROM dbo.GPetItem
	WHERE ItemID = @iPetID_
	and Promotion = @iPromotion_
	and ItemID = @iPetItemID_
)
begin select @iOK = -4 goto end_proc end

begin transaction
    INSERT INTO dbo.GMessageName
    with (updlock)
    (
		ItemID
    ,   Offset
    ,	[Name]
    ,	MessageID
	)
	select
	    @iPetID_
    ,   @iPromotion_
    ,   @strPetName_
    ,   2
    
    if @@error <> 0
    begin select @iOK = -101 goto fail_tran end
    
    INSERT INTO dbo.GPetItem
    with (updlock)
    (
		PetID
    ,	Promotion
    ,	ItemID
    ,	ItemType
    ,	Factor
	)
	select
	    @iPetID_
    ,   @iPromotion_
    ,   @iPetItemID_
    ,   1
    ,   @iFactor_

    if @@error <> 0
    begin select @iOK = -102 goto fail_tran end
commit transaction

goto end_proc

fail_tran:
    rollback transaction

end_proc:
    select @iOK as OK


