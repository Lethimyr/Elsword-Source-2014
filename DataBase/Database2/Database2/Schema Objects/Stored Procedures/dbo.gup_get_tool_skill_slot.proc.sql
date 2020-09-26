CREATE PROCEDURE [dbo].[gup_get_tool_skill_slot]
	@iUnitUID_ [bigint]
AS
SET NOCOUNT ON;

DECLARE @skill_slot TABLE
(
    slot01 int not null,
    slot01name nvarchar(50),
    slot02 int not null,
    slot02name nvarchar(50),
    slot03 int not null,
    slot03name nvarchar(50)
)

INSERT INTO @skill_slot(slot01,slot02,slot03)
    SELECT Slot01, Slot02, Slot03
        FROM dbo.GSkillSlot WITH(NOLOCK) 
            WHERE UnitUID = @iUnitUID_


UPDATE a
    SET a.slot01name = b.name
        FROM @skill_slot as a
            JOIN dbo.GSkillCode as b WITH(NOLOCK) 
				ON a.slot01 = b.code
UPDATE a
    SET a.slot02name = b.name
        FROM @skill_slot as a
            JOIN dbo.GSkillCode as b WITH(NOLOCK) 
				ON a.slot02 = b.code
UPDATE a
    SET a.slot03name = b.name
        FROM @skill_slot as a
            JOIN dbo.GSkillCode as b WITH(NOLOCK) 
				ON a.slot03 = b.code

SELECT	slot01name, slot02name, slot03name
    FROM @skill_slot


