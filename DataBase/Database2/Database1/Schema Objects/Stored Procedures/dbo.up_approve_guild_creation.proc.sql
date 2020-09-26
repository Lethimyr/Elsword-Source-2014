CREATE PROCEDURE [dbo].[up_approve_guild_creation]
	@iGuildId_ [int],
	@iOK_ [int] OUTPUT
AS
exec dbo.up_log_set_proc_count 'up_approve_guild_creation'

SET NOCOUNT ON

DECLARE     @strLoad        nvarchar(20),
            @i10LevelUser   int,
            @iGoodsUID      int,
            @iNeedGP        int,
            @iGuildItemID   int

SET @iNeedGP = 40000

--SET @iGuildItemID = 99999

-- 요청한 길드의 존재, 승인 여부 확인
IF NOT EXISTS(SELECT * FROM dbo.GuildInfo(NOLOCK) WHERE GuildId = @iGuildId_ AND Approval = 0)
BEGIN
    SET @iOK_ = -1
    RETURN @iOK_
END
SELECT @strLoad = LoadLogin, @i10LevelUser = NumMembers FROM dbo.GuildInfo(NOLOCK) WHERE GuildId = @iGuildId_


-- 요청한 길드에 가입원 중 레벨10 이상인 사용자 수가 5 이상인가?
/*
SELECT @i10LevelUser = COUNT(*) FROM dbo.guilduser as a(NOLOCK)
	WHERE a.guildid = @iGuildID_ AND (SELECT COUNT(level) FROM dbo.Characters as b(NOLOCK) WHERE a.login = b.login AND level >= 10) > 0
*/

IF @i10LevelUser < 5
BEGIN
    SET @iOK_ = -2
    GOTO lbFail
END 

UPDATE dbo.GuildInfo SET Approval = 1 WHERE GuildId = @iGuildId_
IF @@ERROR <> 0
    GOTO lbFail

GOTO lbSucc

lbFail:
-- 승인이 실패할 경우
DECLARE @iOK int
EXEC dbo.up_delete_guild @iGuildId_, @iOK OUTPUT
IF @iOK = 0
BEGIN
    -- 해당 load에 대해 gp를 반환해 줌
    UPDATE dbo.Users WITH (UPDLOCK) SET GamePoint = dbo.zcGamePoint(GamePoint + @iNeedGP) WHERE Login = @strLoad
    IF @@ERROR <> 0
        SET @iOK_ = -4
END
ELSE
    SET @iOK_ = -5

RETURN @iOK_

lbSucc:
SET @iOK_ = 0
RETURN @iOK_


