CREATE PROCEDURE [dbo].[gup_get_tool_item_list]
	@iUnitUID_ [bigint]
AS
SET NOCOUNT ON;

DECLARE @ItemList TABLE
(
    ItemUID int not null primary key,
    ItemID int not null,
    UsageType tinyint not null default(0),
    Quantity int not null default(0),
	Endurance int not null default(-1),
    InventoryCategory tinyint not null,
    SlotID tinyint not null,
	ItemName nvarchar(50) 
)

INSERT INTO @ItemList(ItemUID, ItemID, InventoryCategory, SlotID)
    SELECT ItemUID, ItemID, InventoryCategory, SlotID
        FROM dbo.GItem WITH(NOLOCK)
            WHERE UnitUID = @iUnitUID_ AND Deleted = 0

UPDATE a
    SET a.UsageType = 2, a.Quantity = b.Quantity
        FROM @ItemList as a
            JOIN dbo.GItemQuantity as b WITH(NOLOCK) ON a.ItemUID = b.ItemUID

UPDATE a
    SET a.UsageType = 1, a.Endurance = b.Endurance
        FROM @ItemList as a
            JOIN dbo.GItemEndurance as b WITH(NOLOCK) ON a.ItemUID = b.ItemUID

UPDATE a
	SET a.ItemName = b.ItemName
		FROM @ItemList as a 
			JOIN dbo.GItemCode as b with (nolock)
				ON a.ItemID = b.ItemCode
			
SELECT  InventoryCategory
,		ItemName
,       Quantity
,       SlotID
,		ItemUID
,       UsageType
,		Endurance
,       ItemID
    FROM @ItemList order by inventorycategory


