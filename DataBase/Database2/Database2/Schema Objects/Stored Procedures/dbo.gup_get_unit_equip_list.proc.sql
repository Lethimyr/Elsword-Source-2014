CREATE PROCEDURE [dbo].[gup_get_unit_equip_list]
	@iUnitUID_ [bigint]
AS
SET NOCOUNT ON;

SELECT ItemUID
    FROM dbo.GUnitEquip WITH(NOLOCK)
        WHERE UnitUID = @iUnitUID_


