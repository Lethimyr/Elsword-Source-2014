CREATE PROCEDURE [dbo].[gup_get_tool_quest_doing]
	@iUnitUID_ [bigint]
AS
SET NOCOUNT ON;

Select  b.Name, a.Regdate, a.questID
	from dbo.gquests as a, dbo.GQuestCode as b with (nolock)
		where a.unituid = @iUnitUID_ and a.questID = b.code
			option (maxdop 1)


