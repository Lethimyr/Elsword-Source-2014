CREATE PROCEDURE [dbo].[up_leave_guild]
	@strReqLogin_ [nvarchar](20),
	@strLogin_ [nvarchar](20),
	@iGuildId_ [int],
	@iOK_ [int] OUTPUT
AS
exec dbo.up_log_set_proc_count 'up_leave_guild'

SET NOCOUNT ON

DECLARE @iGuildId        int,
        @iUserLevel      int,
        @iNumMembers     int

-- 탈퇴 요청자의 존재 유무 확인
IF NOT EXISTS(SELECT * FROM dbo.Users(NOLOCK) WHERE Login = @strLogin_)
BEGIN
    SET @iOK_ = -1
    RETURN @iOK_
END

-- 요청자의 권한 확인
IF @strReqLogin_ <> @strLogin_ AND
   NOT EXISTS(SELECT * FROM dbo.GuildUser(NOLOCK) WHERE Login = @strReqLogin_ AND GuildId = @iGuildId_ AND UserLevel >= 3 )
BEGIN
    SET @iOK_ = -5
    RETURN @iOK_
END


-- 탈퇴 요청자가 해당 길드에 가입되어 있는지 확인
IF NOT EXISTS(SELECT * FROM dbo.GuildUser(NOLOCK) WHERE Login = @strLogin_ AND GuildId = @iGuildId_)
BEGIN
    SET @iOK_ = -2
    RETURN @iOK_
END
SELECT @iGuildId = GuildId, @iUserLevel = UserLevel FROM dbo.GuildUser(NOLOCK) WHERE Login = @strLogin_

-- 길드의 load(master)은 탈 할 수 없음
IF @iUserLevel = 5
BEGIN
    SET @iOK_ = -3
    RETURN @iOK_
END

-- 탈퇴 요청자가 가입한 길드가 존재하는지 확인: 필요없는 코드이나...
IF NOT EXISTS(SELECT * FROM dbo.GuildInfo(NOLOCK) WHERE GuildId = @iGuildId)
BEGIN
    SET @iOK_ = -4
    RETURN @iOK_
END

BEGIN TRAN
    SELECT @iNumMembers = NumMembers FROM dbo.GuildInfo(NOLOCK) WHERE GuildId = @iGuildId    

   --탈퇴전 길드 정보를 저장한다.
    DECLARE @iUserGuildPoint int, 
                      @strGuildName nvarchar(12)
 
   SELECT @strGuildName = Name FROM dbo.GuildInfo(nolock) where Guildid = @iGuildId
   SELECT @iUserGuildPoint = Point FROM dbo.GuildUser(nolock) where Guildid = @iGuildId and login = @strLogin_

   INSERT INTO dbo.DelGuildUser( GuildID, GuildName, Login, Point, RegDate  ) 
      VALUES( @iGuildId, @strGuildName, @strLogin_, @iUserGuildPoint, GETDATE()  ) 
    IF @@ERROR <> 0
        GOTO lbTran_Abort

    -- 길드에서 탈퇴 처리 함
    DELETE FROM dbo.GuildUser WHERE login = @strLogin_
    IF @@ERROR <> 0
        GOTO lbTran_Abort

    -- 길드 가입자 수를 감소
    IF @iNumMembers > 0
        SET @iNumMembers = @iNumMembers - 1

    UPDATE dbo.GuildInfo SET NumMembers = @iNumMembers WHERE GuildId = @iGuildId
    IF @@ERROR <> 0
        GOTO lbTran_Abort

COMMIT TRAN
GOTO lbTran_Success

lbTran_Abort:
ROLLBACK TRAN
SET @iOK_ = -9
RETURN @iOK_

lbTran_Success:
SET @iOK_ = 0
RETURN @iOK_


