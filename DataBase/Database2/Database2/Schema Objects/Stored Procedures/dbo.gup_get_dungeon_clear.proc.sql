CREATE PROCEDURE [dbo].[gup_get_dungeon_clear]
	@iUnitUID_ [bigint]
AS
SET NOCOUNT ON;

SELECT GameMode, MaxScore, MaxTotalRank, CAST( RegDate AS nvarchar(19)) 
	FROM dbo.GDungeonClear( NOLOCK ) 
		WHERE UnitUID = @iUnitUID_


