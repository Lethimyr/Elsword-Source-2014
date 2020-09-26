CREATE PROCEDURE [dbo].[up_admin_select_user_data]
	@strLogin_ [nvarchar](20)
AS
exec dbo.up_log_set_proc_count 'up_admin_select_user_data'

SET NOCOUNT ON

DECLARE  @iLevel0   int,
         @iLevel1   int,
         @iLevel2   int

IF EXISTS( SELECT * FROM dbo.Users(nolock) where login = @strLogin_ ) 
BEGIN
SELECT @iLevel0 = Level FROM dbo.Characters(NOLOCK) WHERE Login = @strLogin_ AND CharType = 0
SELECT @iLevel1 = Level FROM dbo.Characters(NOLOCK) WHERE Login = @strLogin_ AND CharType = 1
SELECT @iLevel2 = Level FROM dbo.Characters(NOLOCK) WHERE Login = @strLogin_ AND CharType = 2

SELECT 0, Login, firstLogin, lastLogin, gamePoint, -1, @iLevel0, @iLevel1, @iLevel2
    FROM dbo.users(NOLOCK) WHERE Login = @strLogin_
END
ELSE
BEGIN
SELECT -1
END


