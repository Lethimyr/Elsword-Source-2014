CREATE PROCEDURE [dbo].[gup_get_tool_dungeon_doing]
	@iUnitUID_ [bigint]
AS
SET NOCOUNT ON;

SELECT b.Name, a.Regdate, a.GameMode
FROM dbo.gdungeonclear as a WITH (NOLOCK), dbo.gdungeoncode as b WITH (NOLOCK)
WHERE a.unituid = @iUnitUID_ AND a.GameMode = b.Code


