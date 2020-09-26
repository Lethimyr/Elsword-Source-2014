CREATE PROCEDURE [dbo].[up_join_guildState]
	@strLogin_ [nvarchar](20),
	@CN_ [nvarchar](12),
	@iGuildId_ [int],
	@Contents_ [nvarchar](200),
	@Curl_ [nvarchar](20),
	@iOK_ [int] OUTPUT
AS
exec dbo.up_log_set_proc_count 'up_join_guildState'

SET NOCOUNT ON

DECLARE @iJoinItemId        int,
        @iJoinItemUID       int,
        @iMaxNumMembers     int,
	    @Character_         tinyint,
	    @Str                nvarchar(500),
	    @Level_             int

SET @iJoinItemId       = 77777  -- 길드에 가입할 때 필요한 아이템
SET @iMaxNumMembers    = 200    -- 최대 길드원 수

-- 가입자의 존재 유무 확인
IF NOT EXISTS(SELECT * FROM dbo.Users(NOLOCK) WHERE Login = @strLogin_)
BEGIN
    SET @iOK_ = -1
    RETURN @iOK_
END

-- 가입자가 길드에 이미 가입하고 잇는지 확인
IF EXISTS(SELECT * FROM dbo.GuildUser(NOLOCK) WHERE Login = @strLogin_)
BEGIN
    SET @iOK_ = -2
    RETURN @iOK_
END

IF EXISTS(SELECT * FROM dbo.GuildUserState(NOLOCK) WHERE ID=@strLogin_ AND Curl=@Curl_)
BEGIN
    SET @iOK_ = -4
    RETURN @iOK_
END

-- 가입하고자 하는 길드가 존재 하는지 확인
-- 길드원의 수가 초과 하지 않는지 확인
IF NOT EXISTS(SELECT * FROM dbo.GuildInfo(NOLOCK) WHERE GuildId = @iGuildId_ AND NumMembers < @iMaxNumMembers AND Approval=1)
BEGIN
    SET @iOK_ = -3
    RETURN @iOK_
END

BEGIN TRAN
    SELECT TOP 1 @Level_ = level, @Character_ = CharType FROM dbo.Characters(NOLOCK) WHERE login = @strLogin_ ORDER BY Exp DESC

			    -- 가입 요청자를 길드에 추가 함
			    INSERT INTO dbo.GuildUserState ([ID],CN,GuildId,Contents,[Character],[Level],Curl)
			           VALUES (@strLogin_, @CN_,@iGuildId_,@Contents_,@Character_,@Level_,@Curl_) 
			
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


