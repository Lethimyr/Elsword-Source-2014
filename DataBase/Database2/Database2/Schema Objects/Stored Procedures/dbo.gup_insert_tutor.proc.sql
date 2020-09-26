CREATE PROCEDURE [dbo].[gup_insert_tutor]
	@TeacherUID	bigint	
,	@StudentUID	bigint	
,	@iOK			int = 0
AS
SET NOCOUNT ON;


DECLARE @sdtNow	smalldatetime
SELECT	@sdtNow	= GETDATE()


--	한 스승이 제자 3명 초과
IF 3 <= (SELECT COUNT(*) FROM dbo.GTutor WITH (NOLOCK) WHERE TeacherUID = @TeacherUID AND Deleted = 0)
	BEGIN	SELECT	@iOK = -2	GOTO FAIL_TRAN	END


--	제자가 이미 다른 스승과 사제관계를 맺고 있음
IF EXISTS (SELECT * FROM dbo.GTutor WITH (NOLOCK) WHERE StudentUID = @StudentUID AND Deleted = 0)
	BEGIN	SELECT	@iOK = -3	GOTO FAIL_TRAN	END


BEGIN TRAN
	
	INSERT INTO dbo.GTutor (TeacherUID,	StudentUID, RegDate, LastDate, DelDate)
		SELECT @TeacherUID, @StudentUID, @sdtNow, @sdtNow, @sdtNow
	IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
		BEGIN	SELECT	@iOK = -1	GOTO FAIL_TRAN	END

COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


