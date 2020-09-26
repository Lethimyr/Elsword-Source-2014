CREATE PROCEDURE [dbo].[up_verify_game_server]
	@strServerName_ [nvarchar](20),
	@strServerIP_ [nvarchar](15),
	@iServerPort_ [int],
	@iExtraFlag_ [int],
	@iOK [int] = 0
AS
SET NOCOUNT ON;

IF NOT EXISTS(SELECT * FROM dbo.ConnectStatusDB(NOLOCK) WHERE ServerName = @strServerName_)
    BEGIN    SELECT @iOK = -1    GOTO END_PROC    END

DECLARE @strServerIP nvarchar(15), @iServerPort int, @iExtraFlag int
SELECT @strServerIP = '', @iServerPort = 0x80000000, @iExtraFlag = 0x80000000
SELECT @strServerIP = ServerIP, @iServerPort = ServerPort, @iExtraFlag = ExtraFlag
    FROM dbo.ConnectStatusDB(NOLOCK)
        WHERE ServerName = @strServerName_

IF @strServerIP_ <> @strServerIP
    BEGIN    SELECT @iOK = -2    GOTO END_PROC    END

IF @iServerPort_ <> @iServerPort
    BEGIN    SELECT @iOK = -3    GOTO END_PROC    END

IF @iExtraFlag_ <> @iExtraFlag
    BEGIN    SELECT @iOK = -4    GOTO END_PROC    END

END_PROC:
SELECT @iOK


