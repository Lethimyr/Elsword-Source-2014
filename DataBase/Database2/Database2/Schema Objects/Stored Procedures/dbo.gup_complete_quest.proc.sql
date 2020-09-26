CREATE PROCEDURE [dbo].[gup_complete_quest]
	@iUnitUID_ [bigint],
	@iQuestID_ [int],
	@bRepeat   [bit],
	@iOK [int] = 0
AS
SET NOCOUNT ON;

--퀘스트가 없으면 지우지 않는다.
IF NOT EXISTS(SELECT * FROM dbo.GQuests WITH(NOLOCK) WHERE UnitUID = @iUnitUID_ AND QuestID = @iQuestID_ )
    BEGIN    SELECT @iOK = -1    GOTO END_PROC    END

--삭제 전 퀘스트 시작시간을 받아온다.
DECLARE @sdtBegin	smalldatetime
SELECT  @sdtBegin = RegDate
    FROM dbo.GQuests WITH(NOLOCK) WHERE UnitUID = @iUnitUID_ AND QuestID = @iQuestID_

-- 완료시간을 넣어주기위해
DECLARE @sdtComplete smalldatetime
SELECT  @sdtComplete = GetDate()

BEGIN TRAN
	IF @bRepeat = 1
		BEGIN
			INSERT INTO dbo.GQuests_Repeat(
                          UnitUID
,                         QuestID
,                         RegDate
,                         CompleteDate
                          )
                SELECT  @iUnitUID_
,                       @iQuestID_
,                       @sdtBegin
,                       @sdtComplete
            IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
                BEGIN    SELECT @iOK = -11    GOTO FAIL_TRAN    END
		END
	ELSE
		BEGIN
			INSERT INTO dbo.GQuests_Complete(
                          UnitUID
,                         QuestID
,                         RegDate
,                         CompleteDate
                          )
                SELECT  @iUnitUID_
,                       @iQuestID_
,                       @sdtBegin
,                       @sdtComplete
            IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
                BEGIN    SELECT @iOK = -12    GOTO FAIL_TRAN    END
		END

--QUEST 완료 처리가 끝나면 삭제한다.
    DELETE dbo.GQuests
        WHERE UnitUID = @iUnitUID_ AND QuestID = @iQuestID_
    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
        BEGIN    SELECT @iOK = -13    GOTO FAIL_TRAN    END

COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


