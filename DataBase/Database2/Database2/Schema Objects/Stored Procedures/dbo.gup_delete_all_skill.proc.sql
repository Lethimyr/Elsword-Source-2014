/*
	2008.08.05
	SUTAMTWO
	스킬 슬롯 초기화
*/



CREATE PROCEDURE  [dbo].[gup_delete_all_skill] 
	@iUnitUID_	[bigint]
,	@iSPointMax	[int]
,	@iBasicSkill[int]
,	@iOK [int] = 0
AS
SET NOCOUNT ON;

DECLARE @iRowCount int
SELECT	@iRowCount = COUNT(*) 
	FROM dbo.GSkill 
		WHERE UnitUID = @iUnitUID_ 

BEGIN TRAN
	-- 습득 스킬 지우기
    DELETE FROM dbo.GSkill
        WHERE UnitUID = @iUnitUID_
			AND SKILLID NOT IN (@iBasicSkill)

    IF @@ERROR <> 0 OR @@ROWCOUNT > @iRowCount
        BEGIN    SELECT @iOK = -11    GOTO FAIL_TRAN    END

	-- 장착 스킬 지우기
	UPDATE dbo.GSkillSlot WITH( ROWLOCK )
		SET Slot01 = @iBasicSkill, Slot02 = 0, Slot03 = 0
			WHERE UnitUID = @iUnitUID_
	IF @@ERROR <> 0 OR @@ROWCOUNT > 1
		BEGIN	SELECT @iOK = -12	GOTO FAIL_TRAN	END


	IF EXISTS (SELECT * FROM dbo.GSkillSlot2 WITH (NOLOCK) WHERE UnitUID = @iUnitUID_)
		BEGIN
			UPDATE dbo.GSkillSlot2 WITH( ROWLOCK )
				SET Slot01 = 0, Slot02 = 0, Slot03 = 0
					WHERE UnitUID = @iUnitUID_
				IF @@ERROR <> 0 OR @@ROWCOUNT > 1
					BEGIN	SELECT @iOK = -14	GOTO FAIL_TRAN	END
		END


	-- 스킬 포인트 보상
	UPDATE dbo.GUnit 
		SET SPoint = @iSPointMax
			WHERE UnitUID = @iUnitUID_
	IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
		BEGIN	SELECT @iOK = -13	GOTO FAIL_TRAN	END

COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


