CREATE PROCEDURE [dbo].[gup_insert_skill_slot]
	@iUnitUID_ [bigint]
,	@iSlot01 [int]
,	@iSlot02 [int]
,	@iSlot03 [int]
,	@iOK [int] = 0
AS
SET NOCOUNT ON;

BEGIN TRAN
	--이미 등록되어 있으면 업데이트
	IF EXISTS(SELECT * FROM dbo.GSkillSlot WITH(NOLOCK) WHERE UnitUID = @iUnitUID_ )
		BEGIN	
			UPDATE dbo.GSkillSlot WITH( ROWLOCK )
					SET Slot01 = @iSlot01, 
						Slot02 = @iSlot02, 
						Slot03 = @iSlot03
					WHERE UnitUID = @iUnitUID_
				IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
				BEGIN	SELECT @iOK = -11	GOTO FAIL_TRAN	END	    
		END
	--없으면 생성후 등록
	ELSE
		BEGIN
			INSERT INTO dbo.GSkillSlot ( UnitUID, Slot01, Slot02, Slot03 )
					SELECT  @iUnitUID_, @iSlot01, @iSlot02, @iSlot03
				IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
					BEGIN    SELECT @iOK = -12    GOTO FAIL_TRAN    END
		END

COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


