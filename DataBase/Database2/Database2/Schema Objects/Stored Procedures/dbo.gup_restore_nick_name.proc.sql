CREATE PROCEDURE [dbo].[gup_restore_nick_name]
    @iUserUID_			[bigint]
,	@iUnitUID_			[bigint]
,	@strNewNickName_	[nvarchar](16)
,	@iOK				[int] = 0
AS
SET NOCOUNT ON;


-- 해당 유닛이 없는 경우.
IF NOT EXISTS( SELECT * FROM dbo.GUnit WITH( NOLOCK ) WHERE UserUID = @iUserUID_ AND UnitUID = @iUnitUID_ AND Deleted = 0 )
BEGIN
    SELECT @iOK = -1
    GOTO END_PROC
END

DECLARE @strNickName    nvarchar(16)
,		@sdtNow			smalldatetime

SELECT @sdtNow = GetDate()

SELECT @strNickName = NickName FROM dbo.GUnitNickName WITH( NOLOCK ) WHERE UnitUID = @iUnitUID_

-- 닉네임 복구 대상이 아니다.
IF @strNickName <> N'__DELETED__'
BEGIN
    SELECT @iOK = -2
    GOTO END_PROC
END

-- 동일한 닉네임이 존재하는 경우
IF EXISTS( SELECT * FROM dbo.GUnitNickName WITH(NOLOCK) WHERE NickName = @strNewNickName_ )
BEGIN
    SELECT @iOK = -3
    GOTO END_PROC
END

-- 삭제된 닉네임 중 해당 닉네임이 존재하는 경우
IF EXISTS( SELECT * FROM dbo.GDeletedNickName with (nolock) WHERE NickName = @strNewNickName_ )
BEGIN
    SELECT @iOK = -4
    GOTO END_PROC
END

BEGIN TRAN

    UPDATE dbo.GUnitNickName
        SET NickName = @strNewNickName_, RegDate = @sdtNow
        WHERE UnitUID = @iUnitUID_
	IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
	BEGIN    
		SELECT @iOK = -5
		GOTO FAIL_TRAN
	END

COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


