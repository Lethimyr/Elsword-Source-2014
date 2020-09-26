CREATE PROCEDURE [dbo].[up_update_connected_user]
	@strServerName_ [nvarchar](20),
	@strWhereIs_ [nvarchar](20),
	@iId_ [int],
	@strLogin_ [nvarchar](20)
AS
exec dbo.up_log_set_proc_count 'up_update_connected_user'

SET NOCOUNT ON

DECLARE @strSQL varchar(250)
-- 주의: ' ' 공백이 존재 함
IF @strWhereIs_ != ' '
	SET @strSQL = 'UPDATE ' + @strServerName_ + ' SET whereis = ''' + @strWhereIs_ + ''', id = ' + STR( @iId_ )  + ' WHERE login = ''' + @strLogin_ + ''''
ELSE
	SET @strSQL = 'UPDATE ' + @strServerName_ + ' SET id = ' + STR( @iId_ )  + ' WHERE login = ''' + @strLogin_ + ''''
EXEC( @strSQL )


