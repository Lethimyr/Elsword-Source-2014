CREATE PROCEDURE [dbo].[gup_get_inventory_size]
	@iUnitUID_ [bigint]
AS
SET NOCOUNT ON;

SELECT InventoryCategory, SUM(NumSlot)
	FROM dbo.GItemInventorySize WITH (NOLOCK) 
		WHERE UnitUID = @iUnitUID_
			GROUP BY NumSlot, InventoryCategory


