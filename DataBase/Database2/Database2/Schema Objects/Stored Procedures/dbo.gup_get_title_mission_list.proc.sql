create PROCEDURE [dbo].[gup_get_title_mission_list]
	@iUnitUID	bigint
AS
SET NOCOUNT ON;

SELECT  TitleID
,       SubMission1
,       SubMission2
,       SubMission3
,       SubMission4
,       SubMission5
    FROM dbo.GTitle_Mission WITH (NOLOCK)
        WHERE UnitUID = @iUnitUID


