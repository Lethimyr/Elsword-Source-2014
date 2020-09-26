CREATE PROCEDURE [dbo].[up_get_nickname]
	@strName_ [nvarchar](20)
AS
exec dbo.up_log_set_proc_count 'up_get_nickname'

SET NOCOUNT ON

IF NOT EXISTS(SELECT * FROM dbo.users(NOLOCK) WHERE Login = @strName_ )
BEGIN
	--이름이 존재하지 않음.(-1)
	SELECT -1
	RETURN
END

IF NOT EXISTS( SELECT * FROM dbo.nicknames(NOLOCK) WHERE Login = @strName_ )
BEGIN
	SELECT 0, 'null '
	RETURN
END
ELSE
BEGIN
	SELECT 0, nick FROM dbo.nicknames(NOLOCK) WHERE Login = @strName_
	RETURN
END


