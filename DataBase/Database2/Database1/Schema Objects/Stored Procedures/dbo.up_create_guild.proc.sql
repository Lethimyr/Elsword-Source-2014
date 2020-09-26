CREATE PROCEDURE [dbo].[up_create_guild]
	@strName_ [nvarchar](20),
	@strLoad_ [nvarchar](20),
	@strExplanation_ [nvarchar](300),
	@iMark_ [int],
	@strEmail_ [nvarchar](50),
	@strURL_ [nvarchar](20),
	@iOK_ [int] OUTPUT,
	@iGuildId_ [int] OUTPUT
AS
exec dbo.up_log_set_proc_count 'up_create_guild'

SET NOCOUNT ON

DECLARE     @iGuildID       int

DECLARE @iNeedGP int

SET @iNeedGP = 40000

-- 생성하고자 하는 길드와 같은 이름, Mark를 가진 길드가 존재 하는지 확인
DECLARE @iMark int
EXEC @iMark = dbo.up_mark2int @iMark_

IF EXISTS(SELECT * FROM dbo.GuildInfo(NOLOCK) WHERE Name = @strName_ OR Mark = @iMark )
BEGIN
    SET @iOK_ = -1
    RETURN @iOK_
END

-- 길드 생성자가 존재하는지 확인
-- 길드 생성자의 레벨이 10이상인지 확인
-- GP가 충분한지 확인
IF NOT EXISTS(SELECT * FROM dbo.users as a(NOLOCK)
	WHERE a.login = @strLoad_ AND a.gamePoint >= @iNeedGP
		AND (SELECT COUNT(b.level) FROM dbo.Characters as b(NOLOCK) WHERE a.login = b.login AND b.level >= 30) > 0)
BEGIN
    SET @iOK_ = -2
    RETURN @iOK_
END

-- 길드 생성자가 이미 길드에 가입 되어 있는지 확인
IF EXISTS(SELECT Login FROM dbo.GuildUser(NOLOCK) WHERE Login = @strLoad_)
BEGIN
    SET @iOK_ = -3
    RETURN @iOK_
END

-- 등록된 클럽 url 인지 확인
IF EXISTS(SELECT cafe_url FROM dbo.GuildInfo(NOLOCK) WHERE cafe_url = @strURL_)
BEGIN
    SET @iOK_ = -4
    RETURN @iOK_
END

BEGIN TRAN
    DECLARE @iGP int
    SELECT @iGP = GamePoint FROM dbo.Users(NOLOCK) WHERE Login = @strLoad_

    IF @iGP < @iNeedGP
        GOTO lbTran_Abort
    UPDATE dbo.Users SET GamePoint = GamePoint - @iNeedGP WHERE Login = @strLoad_
    IF @@ERROR <> 0
        GOTO lbTran_Abort

    INSERT INTO dbo.GuildInfo (Name, LoadLogin, Explanation, Mark, Email,cafe_url) 
           VALUES ( @strName_, @strLoad_, @strExplanation_, @iMark, @strEmail_ ,@strURL_)
    IF @@ERROR <> 0
        GOTO lbTran_Abort

    -- 추가된 길드의 ID 구하기
    SELECT @iGuildID = MAX(GuildID) FROM dbo.GuildInfo(NOLOCK)
    IF @@ERROR <> 0
        GOTO lbTran_Abort
    
    SET @iGuildId_ = @iGuildID

    -- 길드 추가시 Notice 테이블에 길드를 위한 메시지 보드가 필요함
    DECLARE @iCount int
    SET @iCount = 0
    WHILE @iCount < 5
    BEGIN
        INSERT INTO dbo.Notice(GuildId, MsgId) VALUES ( @iGuildID, @iCount )
        IF @@ERROR <> 0
            GOTO lbTran_Abort
        SET @iCount = @iCount + 1
    END

    -- Load를 길드에 추가 함(load 권한 부여)
    INSERT INTO dbo.GuildUser (Login, GuildId, UserLevel) VALUES (@strLoad_, @iGuildID, 5)
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


