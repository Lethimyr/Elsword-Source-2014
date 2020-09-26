-- 스승이 1, 제자가 0
CREATE PROCEDURE [dbo].[gup_get_tutor] 
	@bIsTeacher	bit
,	@iUnitUID	bigint	
,	@iOK		int = 0
AS
SET NOCOUNT ON;

DECLARE @sdtNow	smalldatetime
SELECT	@sdtNow	= GETDATE()

	IF (@bIsTeacher = 1)
		BEGIN
			SELECT a.StudentUID, b.level, c.Nickname, a.lastdate
				FROM dbo.GTutor as a with (nolock)
					JOIN dbo.GUnit as b with (nolock)
						ON a.TeacherUID = @iUnitUID AND a.StudentUID = b.UnitUID
							JOIN dbo.GUnitNickName as c with (nolock)
								ON a.StudentUID = c.UnitUID
									AND a.Deleted = 0
								
		END
	

	IF (@bIsTeacher = 0)
		BEGIN
			SELECT a.TeacherUID, b.level, c.Nickname, a.lastdate
				FROM dbo.GTutor as a with (nolock)
					JOIN dbo.GUnit as b with (nolock)
						ON a.StudentUID = @iUnitUID and a.TeacherUID = b.UnitUID
							JOIN dbo.GUnitNickName as c with (nolock)
								ON a.TeacherUID = c.UnitUID
									AND a.Deleted = 0
			UPDATE dbo.GTutor WITH (UPDLOCK)
				SET LastDate = @sdtNow
					WHERE StudentUID = @iUnitUID
		END


