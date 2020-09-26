CREATE PROCEDURE [dbo].[gup_get_resurrection_stone]
	@iUnitUID_ [bigint],
    @iQuantity [smallint] = 0
AS
SET NOCOUNT ON;

SELECT @iQuantity = Quantity FROM dbo.GResurrectionStone( NOLOCK ) WHERE UnitUID = @iUnitUID_

SELECT @iQuantity


