CREATE PROCEDURE [dbo].[gup_insert_skill]
	@iUnitUID_ [bigint],
	@iSkillID_ [int],
	@iOK [int] = 0
AS
SET NOCOUNT ON;

--이미 배운 스킬인지 체크
IF EXISTS(SELECT * FROM dbo.GSkill WITH(NOLOCK) WHERE UnitUID = @iUnitUID_ AND SkillID = @iSkillID_ )
    BEGIN    SELECT @iOK = -1    GOTO END_PROC    END

DECLARE @sdtNow smalldatetime
SELECT  @sdtNow = GetDate()

BEGIN TRAN
    INSERT INTO dbo.GSkill(
                           UnitUID
,                          SkillID
,                          RegDate
                          )
        SELECT  @iUnitUID_
,               @iSkillID_
,               @sdtNow
    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
        BEGIN    SELECT @iOK = -11    GOTO FAIL_TRAN    END

COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


