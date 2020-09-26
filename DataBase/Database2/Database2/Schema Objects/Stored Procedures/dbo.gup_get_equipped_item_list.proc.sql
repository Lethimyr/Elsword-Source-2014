CREATE PROCEDURE [dbo].[gup_get_equipped_item_list] 
	@iUnitUID_ [bigint]
AS
SET NOCOUNT ON;


SELECT 

	ITM.ItemUID 
,	ITM.ItemID

,	CASE 
    WHEN DUR.Endurance	IS NOT NULL THEN 1
    WHEN QTY.Quantity	IS NOT NULL THEN 2
	ELSE 0
	END as UsageType

,	isnull(QTY.Quantity, 0)
,	isnull(DUR.Endurance, -1)
,	isnull(PER.Period ,	0)
,	PER.ExpirationDate	
,	isnull(ENC.Elevel, 0)
,	isnull(SOC.Socket1, 0)
,	isnull(SOC.Socket2, 0)
,	isnull(SOC.Socket3, 0)
,	isnull(SOC.Socket4, 0)
,	ITM.InventoryCategory	
,	ITM.SlotID

FROM		GItem			AS ITM  WITH(NOLOCK) 
LEFT OUTER JOIN	GItemQuantity	AS QTY	WITH(NOLOCK) ON ITM.ItemUID = QTY.ItemUID	--//CL
LEFT OUTER JOIN	GItemEndurance 	AS DUR	WITH(NOLOCK) ON ITM.ItemUID = DUR.ItemUID	--//CL
LEFT OUTER JOIN	GItemEnchant 	AS ENC	WITH(NOLOCK) ON ITM.ItemUID = ENC.ItemUID	--//CL
LEFT OUTER JOIN	GItemPeriod 	AS PER	WITH(NOLOCK) ON ITM.ItemUID = PER.ItemUID	--//NC
LEFT OUTER JOIN	GItemSocket 	AS SOC	WITH(NOLOCK) ON ITM.ItemUID = SOC.ItemUID	--//NC

WHERE ITM.UnitUID = @iUnitUID_ AND ITM.InventoryCategory = 9 AND ITM.Deleted = 0


