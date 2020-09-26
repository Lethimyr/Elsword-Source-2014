CREATE PROCEDURE [dbo].[up_delete_user_guild_point]
	@ID [nvarchar](24)
AS
exec dbo.up_log_set_proc_count 'up_delete_user_guild_point'

SET NOCOUNT ON

DECLARE   @iTeamGuildPoint  int,
          @iMyGuildPoint    int,
          @GuildID_         int,  
   	      @strResultSQL     nvarchar(200)

--길드 아이디가 있는지 확인 
IF NOT EXISTS(SELECT * FROM dbo.GuildUser(NOLOCK) WHERE Login = @ID)
BEGIN
    SELECT -1
    RETURN
END
--길드 포인트를 얼마나 갖고 있나,어느 길드에 해당하나 조사한다. 
SELECT @iMyGuildPoint = Point, @GuildID_ = GuildId FROM dbo.GuildUser(nolock) WHERE Login = @ID

--해당길드가 존재하나 알아본다.(혹시나..)
IF NOT EXISTS(SELECT * FROM dbo.GuildInfo(NOLOCK) WHERE GuildID = @GuildID_)
BEGIN
    SELECT -2
    RETURN
END



--해당 길드의 길포가 얼마인지 조사한다.
SELECT @iTeamGuildPoint = Point FROM dbo.GuildInfo(nolock) WHERE GuildID = @GuildID_

SET NOCOUNT ON
BEGIN TRAN
  SET  @iTeamGuildPoint = @iTeamGuildPoint - @iMyGuildPoint

    -- 해당 유저의 길드 포인트 초기화
    UPDATE dbo.GuildUser SET Point = 0 WHERE Login = @ID    
    IF @@ERROR <> 0
        GOTO lbTran_Abort
    --소속 길드의 길드 포인트 깎기.
    UPDATE dbo.GuildInfo SET Point = @iTeamGuildPoint  WHERE GuildID = @GuildID_
    IF @@ERROR <> 0
        GOTO lbTran_Abort
    

COMMIT TRAN
GOTO lbTran_Success

lbTran_Abort:
ROLLBACK TRAN
SELECT -9
RETURN

lbTran_Success:
SET @strResultSQL = 'SELECT 0 ' 
EXEC( @strResultSQL )
RETURN


