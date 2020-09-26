CREATE PROCEDURE [dbo].[gup_get_quest_list]
	@iUnitUID_ [bigint]
AS
SET NOCOUNT ON;

SELECT  QuestID 
,       SubQuest0
,       SubQuest1
,       SubQuest2
,       SubQuest3
,       SubQuest4
    FROM dbo.GQuests WITH(NOLOCK)
        WHERE UnitUID = @iUnitUID_


