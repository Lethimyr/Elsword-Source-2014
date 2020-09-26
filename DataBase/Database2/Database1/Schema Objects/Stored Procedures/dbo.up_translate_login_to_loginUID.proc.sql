CREATE PROCEDURE [dbo].[up_translate_login_to_loginUID]
	@strLogin_ [nvarchar](20),
	@iLoginUID [int] = -1
AS
SET NOCOUNT ON;

SELECT @iLoginUID = LoginUID FROM dbo.Users(NOLOCK) WHERE Login = @strLogin_

SELECT @iLoginUID
RETURN @iLoginUID


