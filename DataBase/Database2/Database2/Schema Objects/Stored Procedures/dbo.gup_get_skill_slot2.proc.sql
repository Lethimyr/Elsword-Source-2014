/*
	2008. 04. 21
	sutamtwo
	스킬슬롯 2 정보를 읽어감
	일단 무조건 값을 넘긴 다음,
	유효성 검사는 서버에서 함
	조건 체크문 1줄만 들어가면 DB에서 할 수도 있음
*/

CREATE PROCEDURE [dbo].[gup_get_skill_slot2] 
	@iUnitUID [bigint]
AS
SET NOCOUNT ON;

IF EXISTS (SELECT * FROM dbo.GSkillSlot2 WITH(NOLOCK) WHERE UnitUID = @iUnitUID)
	BEGIN
		SELECT Slot01, Slot02, Slot03, EndDate
			FROM dbo.GSkillSlot2 WITH(NOLOCK) 
				WHERE UnitUID = @iUnitUID
	END
ELSE
	BEGIN
		SELECT 0 AS 'Slot01', 0 as 'Slot02', 0 as 'Slot03', '2000-01-01 00:00:00' as 'EndDate'
	END


