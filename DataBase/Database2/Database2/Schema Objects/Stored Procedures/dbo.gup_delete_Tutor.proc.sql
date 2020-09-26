CREATE PROCEDURE [dbo].[gup_delete_Tutor]
	@TeacherUID	bigint	
,	@StudentUID	bigint	
,	@iOK		int = 0
AS
SET NOCOUNT ON;

IF NOT EXISTS(SELECT * FROM dbo.GTutor WITH (NOLOCK) WHERE StudentUID = @StudentUID AND TeacherUID = @TeacherUID AND Deleted = 0)
	BEGIN	SELECT	@iOK = -1	GOTO FAIL_TRAN	END

DECLARE @sdtNow	smalldatetime
SELECT	@sdtNow	= DATEADD(MI, 1, GETDATE())

BEGIN TRAN
    UPDATE dbo.GTutor WITH (ROWLOCK)
		SET DelDate = @sdtNow
			WHERE (StudentUID = @StudentUID AND TeacherUID = @TeacherUID)
				AND DELETED = 0
    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
        BEGIN    SELECT @iOK = -11    GOTO FAIL_TRAN    END

COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK
-- 이거는 테스트용 입니다