CREATE PROCEDURE [dbo].[gup_get_skill_slot]
	@iUnitUID_ [bigint]
AS
SET NOCOUNT ON;

SELECT  Slot01, Slot02, Slot03 FROM dbo.GSkillSlot WITH(NOLOCK) WHERE UnitUID = @iUnitUID_


