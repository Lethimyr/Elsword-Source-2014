CREATE PROCEDURE [dbo].[up_translate_nick_to_loginUID]
	@strNick_ [nvarchar](24),
	@iLoginUID [int] = -1
AS
SET NOCOUNT ON;

SELECT @iLoginUID = a.LoginUID
    FROM dbo.Users as a(NOLOCK)
        JOIN dbo.NickNames as b(NOLOCK) ON a.Login = b.Login AND b.Nick = @strNick_

SELECT @iLoginUID
RETURN @iLoginUID


