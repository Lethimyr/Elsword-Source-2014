CREATE PROCEDURE [dbo].[gup_delete_unit]
	@iUnitUID_ [bigint],
	@iOK [int] = 0
AS
SET NOCOUNT ON;

-- 해당 유닛이 없는 경우.
IF NOT EXISTS( SELECT * FROM dbo.GUnit WITH( NOLOCK ) WHERE UnitUID = @iUnitUID_ AND Deleted = 0 )
BEGIN
    SELECT @iOK = -1
    GOTO END_PROC
END

-- 닉네임 얻어오기
DECLARE @strNickName nvarchar(16)

SELECT  @strNickName = NickName
    FROM dbo.GUnitNickName WITH(NOLOCK)
        WHERE UnitUID = @iUnitUID_

-- 닉네임이 널이면
IF @strNickName is NULL
    BEGIN    SELECT @iOK = -2    GOTO END_PROC    END

DECLARE @sdtNow smalldatetime
,       @iItemCount int

SELECT  @sdtNow = DateAdd( mi, 1, GetDate() )
,       @iItemCount = COUNT(*)
    FROM GItem WITH(NOLOCK)
        WHERE UnitUID = @iUnitUID_ AND Deleted = 0

BEGIN TRAN
    UPDATE dbo.GUnit WITH(ROWLOCK)
        SET DelDate = @sdtNow
            WHERE UnitUID = @iUnitUID_ AND Deleted = 0
    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
        BEGIN    SELECT @iOK = -11    GOTO END_PROC    END

--	사제 시스템에서 지우기
	UPDATE dbo.GTutor WITH (ROWLOCK)
		SET DelDate = @sdtNow
			WHERE (TeacherUID = @iUnitUID_ OR StudentUID = @iUnitUID_)
				AND DELETED = 0

    UPDATE dbo.GUnitNickName WITH (ROWLOCK)
        SET NickName = NULL
            WHERE UnitUID = @iUnitUID_
    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
        BEGIN    SELECT @iOK = -12    GOTO END_PROC    END


--  닉네임 과거 기록 저장
	INSERT INTO dbo.GDeletedNickNameHistory (NickName,UnitUID, Regdate)
		SELECT @strNickName, @iUnitUID_, @sdtNow
    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
        BEGIN    SELECT @iOK = -14    GOTO END_PROC    END


    UPDATE dbo.GItem WITH(ROWLOCK)
        SET DelDate = @sdtNow
            WHERE UnitUID = @iUnitUID_ AND Deleted = 0
    IF @@ERROR <> 0 OR @@ROWCOUNT <> @iItemCount
        BEGIN    SELECT @iOK = -15    GOTO END_PROC    END
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


