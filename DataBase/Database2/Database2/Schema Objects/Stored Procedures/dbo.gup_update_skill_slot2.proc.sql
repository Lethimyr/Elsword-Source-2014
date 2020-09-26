/*
	2008. 04.21
	sutamtwo
	스킬슬롯 B에 업데이트 시키기
*/

CREATE PROCEDURE [dbo].[gup_update_skill_slot2]
	@iUnitUID_ [bigint]
,	@iSlot01 [int]
,	@iSlot02 [int]
,	@iSlot03 [int]
,	@iOK [int] = 0
AS
SET NOCOUNT ON;

BEGIN TRAN

	UPDATE dbo.GSkillSlot2 WITH ( ROWLOCK )
			SET Slot01 = @iSlot01, 
				Slot02 = @iSlot02, 
				Slot03 = @iSlot03
			WHERE UnitUID = @iUnitUID_
		IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
		BEGIN	SELECT @iOK = -11	GOTO FAIL_TRAN	END	    

COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


