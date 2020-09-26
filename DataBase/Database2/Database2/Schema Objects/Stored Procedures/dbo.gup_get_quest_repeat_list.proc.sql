CREATE PROCEDURE [dbo].[gup_get_quest_repeat_list]
	@iUnitUID_ [bigint]
AS
SET NOCOUNT ON;

SELECT  QuestID
    FROM dbo.GQuests_Repeat WITH(NOLOCK)
        WHERE UnitUID = @iUnitUID_


