CREATE PROCEDURE [dbo].[up_find_guild]
	@strGuildName_ [nvarchar](12)
AS
exec dbo.up_log_set_proc_count 'up_find_guild'

SET NOCOUNT ON

DECLARE @strWildName nvarchar(20)
SET @strWildName = '%' + @strGuildName_ + '%'
SELECT GuildId, Name, LoadLogin,cafe_url FROM dbo.GuildInfo(NOLOCK) WHERE Name like @strWildName AND Approval = 1


