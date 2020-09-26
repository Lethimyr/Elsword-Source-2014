CREATE PROCEDURE [dbo].[gup_get_tc_clear]
	@iUnitUID_ [bigint]
AS
SET NOCOUNT ON;

SELECT TCID, CAST( RegDate AS nvarchar(19)) FROM dbo.GTrainingCenter (NOLOCK) WHERE UnitUID = @iUnitUID_


