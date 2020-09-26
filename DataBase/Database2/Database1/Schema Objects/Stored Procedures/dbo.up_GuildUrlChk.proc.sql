CREATE PROCEDURE [dbo].[up_GuildUrlChk]
	@Curl_ [nvarchar](20),
	@bSucc [int] OUTPUT
AS
exec dbo.up_log_set_proc_count 'up_GuildUrlChk'

SET NOCOUNT ON

	IF EXISTS(SELECT cafe_url FROM dbo.GuildInfo(NOLOCK) WHERE Cafe_URL = @Curl_)
		BEGIN
			SET @bSucc = -1
			RETURN @bSucc
		End
	Else
		BEGIN
			SET @bSucc = 0
			RETURN @bSucc
		End


