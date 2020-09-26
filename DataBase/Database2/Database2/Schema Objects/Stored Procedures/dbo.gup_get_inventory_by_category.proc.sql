CREATE PROCEDURE [dbo].[gup_get_inventory_by_category]
    @iUnitUID_              [bigint],
    @iInventoryCategory_    [tinyint]
AS

SET NOCOUNT ON;

DECLARE @InventoryInfo TABLE
(
    SlotID tinyint not null,
    ItemUID bigint not null,
    ItemID int not null,
    Quantity smallint not null
)

INSERT INTO @InventoryInfo( SlotID, ItemUID, ItemID, Quantity )
    SELECT SlotID, ItemUID, 0, Quantity
        FROM dbo.GItemInventory WITH(NOLOCK)
            WHERE InventoryCategory = @iInventoryCategory_ AND UnitUID = @iUnitUID_ AND ItemUID IS NOT NULL

UPDATE a
    SET a.ItemID = b.ItemID
        FROM @InventoryInfo as a
            JOIN dbo.GItem as b WITH(NOLOCK) ON a.ItemUID = b.ItemUID AND b.Deleted = 0

SELECT SlotID, ItemUID, ItemID, Quantity
    FROM @InventoryInfo


