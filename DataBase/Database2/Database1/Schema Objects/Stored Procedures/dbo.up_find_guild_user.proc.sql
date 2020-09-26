CREATE PROCEDURE [dbo].[up_find_guild_user]
	@strLogin_ [nvarchar](20)
AS
exec dbo.up_log_set_proc_count 'up_find_guild_user'

SET NOCOUNT ON

SELECT A.GuildId, A.Name, A.LoadLogin,A.Cafe_url FROM dbo.GuildInfo AS A(NOLOCK), dbo.GuildUser AS B(NOLOCK) 
WHERE B.Login = @strLogin_ AND A.GuildId = B.GuildId


