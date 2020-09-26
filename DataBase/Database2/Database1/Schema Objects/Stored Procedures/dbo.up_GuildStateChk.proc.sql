CREATE PROCEDURE [dbo].[up_GuildStateChk]
	@GuildID_ [int],
	@bSucc [int] OUTPUT
AS
exec dbo.up_log_set_proc_count 'up_GuildStateChk'

SET NOCOUNT ON

DECLARE @nCode    int

    IF NOT EXISTS(SELECT Approval FROM dbo.GuildInfo(NOLOCK) WHERE GuildID = @GuildID_)
        BEGIN
            SET @bSucc = -1
            RETURN @bSucc
        End
        
        SELECT @nCode = Approval FROM dbo.GuildInfo(nolock) WHERE GuildID = @GuildID_
            IF @nCode = 0
                Begin
                    SET @bSucc = 0
                    Return @bSucc
                End
            Else
                Begin
                    SET @bSucc = 1
                    Return @bSucc
                End


