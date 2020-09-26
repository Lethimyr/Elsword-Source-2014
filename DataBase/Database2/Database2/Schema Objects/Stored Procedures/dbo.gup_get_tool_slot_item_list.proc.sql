CREATE PROCEDURE [dbo].[gup_get_tool_slot_item_list]
	@iUnitUID_ [bigint]
AS
SET NOCOUNT ON;

DECLARE @ItemList TABLE
(
    ItemUID int not null primary key,
    ItemID int not null,
    Quantity int not null default(0),
    InventoryCategory tinyint not null,
    SlotID tinyint not null
,	ItemName nvarchar(50)
)

INSERT INTO @ItemList(ItemUID, ItemID, InventoryCategory, SlotID)
    SELECT ItemUID, ItemID, InventoryCategory, SlotID
        FROM dbo.GItem WITH(NOLOCK)
            WHERE UnitUID = @iUnitUID_ AND Deleted = 0

UPDATE a
    SET a.Quantity = b.Quantity
        FROM @ItemList as a
            JOIN dbo.GItemQuantity as b WITH(NOLOCK) ON a.ItemUID = b.ItemUID

UPDATE a
	SET a.ItemName = b.ItemName
		FROM @ItemList as a
			JOIN game01.dbo.GItemCode as b with(nolock) on a.ItemID = b.ItemCode

SELECT  ItemName
,       Quantity
,       InventoryCategory
,       SlotID
,		ItemID
    FROM @ItemList 
		where inventorycategory = 11
			order by slotID


