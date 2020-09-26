CREATE PROCEDURE [dbo].[up_join_guild]
	@strReqLogin_ [nvarchar](20),
	@strLogin_ [nvarchar](20),
	@iGuildId_ [int],
	@iOK_ [int] OUTPUT
AS
exec dbo.up_log_set_proc_count 'up_join_guild'

SET NOCOUNT ON

DECLARE @iJoinItemId    int,
        @iJoinItemUID   int,
        @iMaxNumMembers int

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

-- 요청자가 길마인지 확인
--IF NOT EXISTS(SELECT * FROM dbo.GuildUser(NOLOCK) WHERE Login = @strReqLogin_ AND GuildId = @iGuildId_ AND UserLevel >= 3 )
IF @strReqLogin_ <> @strLogin_ AND
   NOT EXISTS(SELECT * FROM dbo.GuildUser(NOLOCK) WHERE Login = @strReqLogin_ AND GuildId = @iGuildId_ AND UserLevel >= 3 )
BEGIN
    SET @iOK_ = -4
    RETURN @iOK_
END


-- 가입하고자 하는 길드가 존재 하는지 확인
-- 길드원의 수가 초과 하지 않는지 확인
IF NOT EXISTS(SELECT * FROM dbo.GuildInfo(NOLOCK) WHERE GuildId = @iGuildId_ AND NumMembers < @iMaxNumMembers)
BEGIN
    SET @iOK_ = -3
    RETURN @iOK_
END

-- 가입 요청자 길드 가입 아이템을 가지고 있는지 확인
/*
IF NOT EXISTS(SELECT * FROM dbo.GoodsObjectList(NOLOCK) WHERE OwnerLogin = @strLogin_ AND GoodsId = @iJoinItemId)
BEGIN
    SET @iOK_ = -4
    RETURN @iOK_
END
SELECT @iJoinItemUID = GoodsUID FROM dbo.GoodsObjectList(NOLOCK) WHERE OwnerLogin = @strLogin_ AND GoodsId = @iJoinItemId
*/

BEGIN TRAN
    -- 가입 요청자를 길드에 추가 함
    INSERT INTO dbo.GuildUser (Login, GuildId)
           VALUES (@strLogin_, @iGuildId_)
    IF @@ERROR <> 0
        GOTO lbTran_Abort

    -- 길드 가입자 수를 증가
    UPDATE dbo.GuildInfo SET NumMembers = NumMembers + 1 WHERE GuildId = @iGuildId_
    IF @@ERROR <> 0
        GOTO lbTran_Abort

    --GuildUserState에서 삭제
    DELETE FROM dbo.GuildUserState WHERE ID = @strLogin_

    -- 요청자의 길드 아템을 삭제 함
/*
    DELETE FROM dbo.GoodsObjectList WHERE GoodsUID = @iJoinItemUID
    IF @@ERROR <> 0
        GOTO lbTran_Abort
*/

COMMIT TRAN
GOTO lbTran_Success

lbTran_Abort:
ROLLBACK TRAN
SET @iOK_ = -9
RETURN @iOK_

lbTran_Success:
SET @iOK_ = 0
RETURN @iOK_


