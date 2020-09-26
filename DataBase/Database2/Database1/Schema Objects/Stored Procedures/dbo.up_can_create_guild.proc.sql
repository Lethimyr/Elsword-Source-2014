CREATE PROCEDURE [dbo].[up_can_create_guild]
	@strLogin_ [nvarchar](20),
	@iOK_ [int] OUTPUT,
	@strGuildName_ [nvarchar](12) OUTPUT
AS
exec dbo.up_log_set_proc_count 'up_can_create_guild'

SET NOCOUNT ON

DECLARE @iGuildItemID   int,
        @iGuildId       int

DECLARE @iNeedGP int

SET @iNeedGP = 40000

SET @iGuildItemID = 99999

-- 길드 생성자가 존재하는지 확인
-- 길드 생성자의 레벨이 10이상인지 확인
IF NOT EXISTS(SELECT * FROM dbo.users as a(NOLOCK)
	WHERE a.login = @strLogin_ AND a.gamePoint >= @iNeedGP
		AND (SELECT COUNT(b.level) FROM dbo.Characters as b(NOLOCK) WHERE a.login = b.login AND b.level >= 30) > 0)
BEGIN
    SET @iOK_ = -2
    RETURN @iOK_
END

-- 길드 생성자가 이미 길드에 가입 되어 있는지 확인
IF EXISTS(SELECT * FROM dbo.GuildUser(NOLOCK) WHERE Login = @strLogin_)
BEGIN
    SELECT @iGuildId = GuildId FROM dbo.GuildUser(NOLOCK) WHERE Login = @strLogin_
    SELECT @strGuildName_ = Name FROM dbo.GuildInfo(NOLOCK) WHERE GuildId = @iGuildId
    SET @iOK_ = -3
    RETURN @iOK_
END

-- 길드 생성 아이템을 가지고 있는지 확인
/*
IF NOT EXISTS(SELECT * FROM dbo.GoodsObjectList(NOLOCK) WHERE OwnerLogin = @strLogin_ AND GoodsId = @iGuildItemID)
BEGIN
    SET @iOK_ = -4
    RETURN @iOK_
END
*/

SET @iOK_ = 0
RETURN @iOK_


