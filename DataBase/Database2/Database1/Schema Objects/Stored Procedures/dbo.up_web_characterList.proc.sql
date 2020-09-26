CREATE PROCEDURE [dbo].[up_web_characterList]
	@strLogin_ [nvarchar](20)
AS
SET NOCOUNT ON

SELECT  CharType
    FROM [dbo].[Characters] WITH(NOLOCK)
        WHERE Login = @strLogin_
            ORDER BY CharType


