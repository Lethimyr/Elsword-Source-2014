CREATE PROCEDURE [dbo].[up_translate_nickname_to_login]
	@strNickName_ [nvarchar](24)
AS
exec dbo.up_log_set_proc_count 'up_translate_nickname_to_login'

SET NOCOUNT ON

IF NOT EXISTS(SELECT * FROM dbo.nicknames(NOLOCK) WHERE nick = @strNickName_ )
BEGIN
	--넥네임이 존재하지 않음.(-1)
	SELECT -1, NULL
	RETURN
END

SELECT 0, login FROM dbo.nicknames(NOLOCK) WHERE nick = @strNickName_
RETURN


