CREATE PROCEDURE [dbo].[up_web_exist_extra_characters]
	@strLogin_ [nvarchar](20),
	@nCharID_ [int],
	@iOK [int] = 0
AS
SET NOCOUNT ON;

IF NOT EXISTS(SELECT * FROM dbo.Characters(NOLOCK) WHERE Login = @strLogin_ AND CharType = @nCharID_)
    BEGIN    SELECT @iOK = -1    END

SELECT @iOK
RETURN @iOK


