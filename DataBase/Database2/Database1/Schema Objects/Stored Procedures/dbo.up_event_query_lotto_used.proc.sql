CREATE PROCEDURE [dbo].[up_event_query_lotto_used]
	@strLogin_ [nvarchar](20)
AS
exec dbo.up_log_set_proc_count 'up_event_query_lotto_used'

SET NOCOUNT ON

DECLARE @iRound int

IF EXISTS(SELECT TOP 1 * FROM dbo.EventLottoR(NOLOCK) ORDER BY [Round] DESC)
    BEGIN
        SELECT @iRound = MAX([Round]) FROM dbo.EventLottoR(NOLOCK)
    END
ELSE
    BEGIN
        SELECT @iRound = 1
        INSERT INTO dbo.EventLottoR([Round]) VALUES(@iRound)
    END

SELECT @iRound, LottoID, Lotto1, Lotto2, Lotto3, Lotto4, Lotto5, RegDate FROM dbo.EventLottoN(NOLOCK) WHERE Login = @strLogin_
UNION ALL
SELECT @iRound - 1, LottoID, Lotto1, Lotto2, Lotto3, Lotto4, Lotto5, RegDate FROM dbo.EventLottoP(NOLOCK) WHERE Login = @strLogin_


