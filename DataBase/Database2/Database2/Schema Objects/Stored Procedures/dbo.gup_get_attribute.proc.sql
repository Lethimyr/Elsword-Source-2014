CREATE PROCEDURE [dbo].[gup_get_attribute]
    @iUnitUID [bigint]
AS

SET NOCOUNT ON;
SET XACT_ABORT ON;

SELECT *
	FROM dbo.GItemAttribute	WITH (NOLOCK)
		WHERE ItemUID IN 
			(
				SELECT ItemUID FROM dbo.GItem WITH (NOLOCK) WHERE UnitUID = @iUnitUID AND Inventorycategory IN (1,9) AND Deleted = 0
			)


