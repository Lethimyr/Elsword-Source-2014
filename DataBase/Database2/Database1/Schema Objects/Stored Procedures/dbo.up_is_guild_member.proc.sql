CREATE PROCEDURE [dbo].[up_is_guild_member]
	@strLogin_ [nvarchar](20),
	@iGuildId_ [int],
	@iOK_ [int] OUTPUT
AS
exec dbo.up_log_set_proc_count 'up_is_guild_member'

SET NOCOUNT ON

IF NOT EXISTS(SELECT * FROM dbo.GuildUser(NOLOCK) WHERE Login = @strLogin_ AND GuildId = @iGuildId_)
BEGIN
    SET @iOK_ = -1
    RETURN @iOK_
END

SET @iOK_ = 0
RETURN @iOK_


