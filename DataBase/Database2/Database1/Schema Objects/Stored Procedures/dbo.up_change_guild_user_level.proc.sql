CREATE PROCEDURE [dbo].[up_change_guild_user_level]
	@strLogin_ [nvarchar](20),
	@strReqLogin_ [nvarchar](20),
	@iLevel_ [int],
	@iOK_ [int] OUTPUT
AS
exec dbo.up_log_set_proc_count 'up_change_guild_user_level'

SET NOCOUNT ON

DECLARE @iGuildId        int,
        @iUserLevel      int,
        @iNumGenerals    int,
        @iNumMembers     int,
        @strLoad         nvarchar(20)

-- @iLevel_  5 에서 3으로 변경
IF  @iLevel_ < 0 OR @iLevel_ > 5
BEGIN
    SET @iOK_ = -1
    RETURN @iOK_
END

-- 요청자의 존재 유무 확인
IF NOT EXISTS(SELECT * FROM dbo.Users(NOLOCK) WHERE Login = @strLogin_)
BEGIN
    SET @iOK_ = -2
    RETURN @iOK_
END

-- 요청자가 길드에 가입되어 있는지 확인
IF NOT EXISTS(SELECT * FROM dbo.GuildUser(NOLOCK) WHERE Login = @strLogin_)
BEGIN
    SET @iOK_ = -3
    RETURN @iOK_
END
SELECT @iGuildId = GuildId, @iUserLevel = UserLevel FROM dbo.GuildUser(NOLOCK) WHERE Login = @strLogin_

-- 요청자의 권한에 문제가 없는지 확인
IF NOT EXISTS(SELECT * FROM dbo.GuildUser(NOLOCK) WHERE Login = @strReqLogin_ AND GuildId = @iGuildId AND UserLevel >= 3 )
BEGIN
    SET @iOK_ = -6
    RETURN @iOK_
END

-- 요청자가 가입한 길드가 존재하는지 확인: 필요없는 코드이나...
IF NOT EXISTS(SELECT * FROM dbo.GuildInfo(NOLOCK) WHERE GuildId = @iGuildId)
BEGIN
    SET @iOK_ = -4
    RETURN @iOK_
END
SELECT @strLoad = LoadLogin, @iNumMembers = NumMembers FROM dbo.GuildInfo(NOLOCK) WHERE GuildId = @iGuildId

-- General로 변경할 경우 #general의 수는 #member의 수의 1/10을 넘지 못 한다.
IF @iLevel_ = 3
BEGIN
    SET @iNumGenerals = (SELECT COUNT(*) FROM dbo.GuildUser(NOLOCK) WHERE GuildId = @iGuildId AND UserLevel = 3 )
    IF @iNumGenerals > 1 AND @iNumGenerals >= CAST(@iNumMembers * 0.1 AS int)  
    BEGIN
        SET @iok_ = -5
        RETURN @iOK_
    END
END

BEGIN TRAN

    -- 변경 처리
    UPDATE dbo.GuildUser SET UserLevel = @iLevel_ WHERE Login = @strLogin_
    IF @@ERROR <> 0
        GOTO lbTran_Abort
   
    -- master의 권한이 변경 되는 경우 현재 master의 권한을 가진 사용자의 권한을 knight로 변경 해야 함
    -- 길드 정보에서 master의 이름이 변경
    IF @iLevel_ = 5
    BEGIN
        UPDATE dbo.GuildUser SET UserLevel = 0 WHERE Login = @strLoad
        IF @@ERROR <> 0
            GOTO lbTran_Abort
        UPDATE dbo.GuildInfo SET LoadLogin = @strLogin_ WHERE GuildId = @iGuildId 
    END


COMMIT TRAN
GOTO lbTran_Success

lbTran_Abort:
ROLLBACK TRAN
SET @iOK_ = -9
RETURN @iOK_

lbTran_Success:
SET @iOK_ = 0
RETURN @iOK_


