CREATE PROCEDURE [dbo].[up_event_query_maxbong]
	@strLogin_ [nvarchar](20),
	@iOK [int] = 4
AS
SET NOCOUNT ON;

IF NOT EXISTS(SELECT * FROM dbo.Users(NOLOCK) WHERE Login = @strLogin_)
    BEGIN    SELECT @iOK = 3    GOTO END_PROC    END

DECLARE @iLevel int
SELECT TOP 1 @iLevel = Level FROM dbo.Characters(NOLOCK) WHERE Login = @strLogin_ ORDER BY Exp DESC, CharType ASC

SELECT @iOK = CASE WHEN @iLevel < 3 THEN 2 ELSE 1 END

END_PROC:
SELECT @iOK


