CREATE PROCEDURE [dbo].[getGuildID]
	@Curl_ [nvarchar](20),
	@iOK_ [int] OUTPUT,
	@Name_ [nvarchar](20) OUTPUT
AS
exec dbo.up_log_set_proc_count 'getGuildID'

SET NOCOUNT ON

declare @nGuildID int, @GuildName_ nvarchar(20)

    IF EXISTS(SELECT GuildID FROM dbo.GuildInfo(nolock) WHERE Cafe_URL=@Curl_)
        Begin
            SELECT TOP 1 @nGuildID = GuildID, @GuildName_ = [Name] FROM dbo.GuildInfo(nolock) WHERE Cafe_URL = @Curl_
            SET @iOK_ = @nGuildID
            SET @Name_ = @GuildName_
            Return @iOK_
            Return @Name_
        End

    ELSE
        Begin
            SET @iOK_ = -1
            Return @iOK_
        End


