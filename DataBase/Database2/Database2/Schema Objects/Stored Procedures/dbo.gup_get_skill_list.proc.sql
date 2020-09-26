CREATE PROCEDURE [dbo].[gup_get_skill_list]
	@iUnitUID_ [bigint]
AS
SET NOCOUNT ON;

SELECT  SkillID FROM dbo.GSkill WITH(NOLOCK) WHERE UnitUID = @iUnitUID_


