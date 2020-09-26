CREATE PROCEDURE [dbo].[up_guild_cancel_wait_user]
	@strReqLogin_ [nvarchar](20),
	@strLogin_ [nvarchar](20),
	@iGuildID_ [int]
AS
exec up_log_set_proc_count 'up_guild_cancel_wait_user'

SET NOCOUNT ON

-- 요청자가 길마인지 확인 or 요청자와 대상자가 같은지 확인
IF @strReqLogin_ <> @strLogin_ AND
   NOT EXISTS(SELECT * FROM GuildUser(NOLOCK) WHERE Login = @strReqLogin_ AND GuildId = @iGuildID_ AND UserLevel >= 3 )
BEGIN
    RETURN -1
END

IF NOT EXISTS( SELECT * FROM GuildUserState(NOLOCK) where ID = @strLogin_ )
BEGIN
   RETURN -2
END


IF  @iGuildID_ <> -1 AND 
    NOT EXISTS( SELECT * From GuildUserState(nolock) WHERE GuildID = @iGuildID_ )
BEGIN
    RETURN -3END


BEGIN TRAN
--해당길드에 신청한 것만  삭제
IF @iGuildID_ <> -1
BEGIN
	DELETE FROM GuildUserState WHERE GuildID = @iGuildID_ AND ID = @strLogin_
              IF @@ERROR <> 0 
                GOTO lbTran_Abort

END
ELSE
BEGIN
--일괄삭제
	DELETE FROM GuildUserState WHERE ID = @strLogin_
              IF @@ERROR <> 0 
                GOTO lbTran_Abort
END

COMMIT TRAN

GOTO lbTran_Success


lbTran_Abort:
ROLLBACK TRAN
RETURN	-9

lbTran_Success:
RETURN 0


