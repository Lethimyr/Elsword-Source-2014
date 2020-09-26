CREATE PROCEDURE [dbo].[gup_get_quest_complete_list]
	@iUnitUID_ [bigint]
AS
SET NOCOUNT ON;

SELECT  QuestID
    FROM dbo.GQuests_Complete WITH(NOLOCK)
        WHERE UnitUID = @iUnitUID_


