CREATE PROCEDURE [dbo].[up_GuildUserStateChk]
	@strLogin_ [nvarchar](20),
	@Curl_ [nvarchar](20),
	@bSucc [int] OUTPUT
AS
exec dbo.up_log_set_proc_count 'up_GuildUserStateChk'

SET NOCOUNT ON

IF EXISTS(SELECT * FROM dbo.GuildUserState(NOLOCK) WHERE [ID] = @strLogin_ AND Curl = @Curl_)
	BEGIN
	    SET @bSucc = 0
	    RETURN @bSucc
	END
Else
	BEGIN
		IF EXISTS(SELECT * FROM dbo.GuildUser(NOLOCK) WHERE Login = @strLogin_)
			BEGIN
			    SET @bSucc = -1
			    RETURN @bSucc
			END
		Else
			Begin
				SET @bSucc = 1
				RETURN @bSucc
			End
	END


