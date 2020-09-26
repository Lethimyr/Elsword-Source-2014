CREATE PROCEDURE [dbo].[gup_get_tool_quest_complete]
	@iUnitUID_ [bigint]
AS
SET NOCOUNT ON;

Select  b.Name, a.RegDate, a.CompleteDate, a.questID
	from dbo.gquests_complete as a, dbo.GQuestCode as b with (nolock)
		where a.unituid = @iUnitUID_ and a.questID = b.code
			option (maxdop 1)


