CREATE PROCEDURE [dbo].[gup_delete_user]
	@iUserID	[nvarchar] (16)
,	@iOK [int] = 0
AS
SET NOCOUNT ON;


DECLARE @sdtNow		smalldatetime
,       @iItemCount int
,		@iUserUID	bigint
,		@iUnitCount	int


DECLARE @tabUnitUID TABLE
(
	UnitUID	BIGINT
,	NickName	nvarchar(16)
)


SELECT @iUserUID = UserUID 
	FROM dbo.GUser WITH (NOLOCK)
		WHERE UserID = 	@iUserID



-- 유닛이 하나도 없는 경우
IF NOT EXISTS( SELECT * FROM dbo.GUnit WITH( NOLOCK ) WHERE UserUID = @iUserUID AND DELETED = 0 )
BEGIN
	SELECT @iOK = 0    GOTO END_PROC
END



-- 유닛이 있는 경우

INSERT INTO @tabUnitUID (UnitUID)
	SELECT UnitUID FROM dbo.GUnit WITH (NOLOCK) WHERE UserUID = @iUserUID AND DELETED = 0

UPDATE a
	SET a.NickName = b.NickName
		FROM @tabUnitUID AS A
			JOIN dbo.GUnitNickName AS B WITH (NOLOCK)
				ON A.UnitUID = B.UnitUID

SELECT	@iUnitCount = COUNT(*)
	FROM @tabUnitUID

SELECT  @sdtNow = DateAdd( mi, 1, GetDate() )
,       @iItemCount = COUNT(*)
    FROM dbo.GItem WITH(NOLOCK)
        WHERE UnitUID in (SELECT UnitUID FROM @tabUnitUID)
			AND Deleted = 0


BEGIN TRAN

--	유닛 정보 지우기
    UPDATE dbo.GUnit WITH(ROWLOCK)
        SET DelDate = @sdtNow
            WHERE UnitUID in (SELECT UnitUID FROM @tabUnitUID)
    IF @@ERROR <> 0 OR @@ROWCOUNT <> @iUnitCount
        BEGIN    SELECT @iOK = -11    GOTO END_PROC    END

--	사제 시스템에서 지우기
	UPDATE dbo.GTutor WITH (ROWLOCK)
		SET DelDate = @sdtNow
			WHERE (TeacherUID  in (SELECT UnitUID FROM @tabUnitUID) OR StudentUID in (SELECT UnitUID FROM @tabUnitUID))
				AND DELETED = 0

    UPDATE dbo.GUnitNickName WITH (ROWLOCK)
        SET NickName = NULL
            WHERE UnitUID in (SELECT UnitUID FROM @tabUnitUID)
    IF @@ERROR <> 0 OR @@ROWCOUNT <> @iUnitCount
        BEGIN    SELECT @iOK = -12    GOTO END_PROC    END


--  닉네임 과거 기록 저장
	INSERT INTO dbo.GDeletedNickNameHistory (NickName,UnitUID, Regdate)
		SELECT NickName, UnitUID, @sdtNow
			FROM @tabUnitUID
    IF @@ERROR <> 0 OR @@ROWCOUNT <> @iUnitCount
        BEGIN    SELECT @iOK = -14    GOTO END_PROC    END


    UPDATE dbo.GItem WITH(ROWLOCK)
        SET DelDate = @sdtNow
            WHERE UnitUID in (SELECT UnitUID FROM @tabUnitUID) AND Deleted = 0
    IF @@ERROR <> 0 OR @@ROWCOUNT <> @iItemCount
        BEGIN    SELECT @iOK = -15    GOTO END_PROC    END
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


