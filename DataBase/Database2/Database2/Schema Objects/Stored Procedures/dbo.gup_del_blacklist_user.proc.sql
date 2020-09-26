CREATE PROCEDURE [dbo].[gup_del_blacklist_user]
	@iUnitUID_ [bigint],	
	@strNickName_ [nvarchar](16),
	@iOK [int] = 0
AS
SET NOCOUNT ON;

--존재하는 닉네임인지 확인
DECLARE @iBLUnitUID_ bigint

SET @iBLUnitUID_ = (SELECT UnitUID FROM dbo.GUnitNickName WHERE NickName = @strNickName_ )
    IF @iBLUnitUID_ IS NULL
        BEGIN    SELECT @iOK = -10    GOTO END_PROC    END

--삭제할 유저가 블랙리스트에 있는지 체크
IF NOT EXISTS(SELECT * FROM dbo.GFriendList WITH(NOLOCK) WHERE UnitUID1 = @iUnitUID_ AND UnitUID2 = @iBLUnitUID_ AND Type = 4)
    BEGIN    SELECT @iOK = -1    GOTO END_PROC    END

BEGIN TRAN
    DELETE dbo.GFriendList
        WHERE UnitUID1 = @iUnitUID_ AND UnitUID2 = @iBLUnitUID_ AND Type = 4
    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
        BEGIN    SELECT @iOK = -11    GOTO FAIL_TRAN    END

COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


