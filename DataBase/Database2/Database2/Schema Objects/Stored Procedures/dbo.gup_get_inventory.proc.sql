CREATE PROCEDURE [dbo].[gup_get_inventory]
    @iUnitUID_ [bigint]
AS

SET NOCOUNT ON;

DECLARE @InventoryInfo TABLE
(
    InventoryCategory tinyint not null,
    SlotID tinyint not null,
    ItemUID bigint not null,
    ItemID int not null,
    Quantity smallint not null
)

INSERT INTO @InventoryInfo( InventoryCategory, SlotID, ItemUID, ItemID, Quantity )
    SELECT InventoryCategory, SlotID, ItemUID, 0, Quantity
        FROM dbo.GItemInventory WITH(NOLOCK)
            WHERE UnitUID = @iUnitUID_ AND ItemUID IS NOT NULL

UPDATE a
    SET a.ItemID = b.ItemID
        FROM @InventoryInfo as a
            JOIN dbo.GItem as b WITH(NOLOCK) ON a.ItemUID = b.ItemUID AND b.Deleted = 0

SELECT InventoryCategory, SlotID, ItemUID, ItemID, Quantity
    FROM @InventoryInfo


