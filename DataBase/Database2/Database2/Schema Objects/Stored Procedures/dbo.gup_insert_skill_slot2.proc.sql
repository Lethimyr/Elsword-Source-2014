/*
	2008. 04. 21 
	sutamtwo
	스킬 슬롯 B 를 열어주는 아이템 구매 시,
	dbo.GSkillSlot2 라는 테이블에 0 및 종료시점 Insert 혹은 Update
*/

CREATE PROCEDURE [dbo].[gup_insert_skill_slot2]
	@iUnitUID_ [bigint]
,	@iPeriod [int]
,	@iOK [int] = 0
AS
SET NOCOUNT ON;

DECLARE @sdtEndDate smalldatetime
SELECT	@sdtEndDate = dateadd(dd, @iPeriod, getdate())

BEGIN TRAN
	--이미 등록되어 있으면 끝나는 날짜만 업데이트
	IF EXISTS(SELECT * FROM dbo.GSkillSlot2 WITH(NOLOCK) WHERE UnitUID = @iUnitUID_ )
		BEGIN	
			UPDATE dbo.GSkillSlot2 WITH( ROWLOCK )
					SET Slot01 = 0, 
						Slot02 = 0, 
						Slot03 = 0,
						EndDate = @sdtEndDate
					WHERE UnitUID = @iUnitUID_
				IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
				BEGIN	SELECT @iOK = -11	GOTO FAIL_TRAN	END	    
		END
	--없으면 생성
	ELSE
		BEGIN
			INSERT INTO dbo.GSkillSlot2 ( UnitUID, Slot01, Slot02, Slot03, EndDate )
					SELECT  @iUnitUID_, 0, 0, 0, @sdtEndDate
				IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
					BEGIN    SELECT @iOK = -12    GOTO FAIL_TRAN    END
		END

COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK, @sdtEndDate


