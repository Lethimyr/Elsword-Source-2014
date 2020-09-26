CREATE PROCEDURE [dbo].[up_event_query_lotto_unused_count]
	@strLogin_ [nvarchar](20)
AS
exec dbo.up_log_set_proc_count 'up_event_query_lotto_unused_count'

SET NOCOUNT ON

DECLARE @iOK int

IF NOT EXISTS( SELECT * FROM dbo.EventLottoU(NOLOCK) WHERE login = @strLogin_ )
    BEGIN    
        SELECT @strLogin_, -1
    END
ELSE
    BEGIN
        SELECT login, [Count] FROM dbo.EventLottoU(NOLOCK) WHERE login = @strLogin_
    END


