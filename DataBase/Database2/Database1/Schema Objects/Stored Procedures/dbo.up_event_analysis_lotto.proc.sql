CREATE PROCEDURE [dbo].[up_event_analysis_lotto]
	@iRound_ [int]
AS
exec dbo.up_log_set_proc_count 'up_event_analysis_lotto'

SET NOCOUNT ON

DECLARE @iLotto int

SELECT @iLotto = Lotto FROM dbo.EventLottoR(NOLOCK) WHERE Round = @iRound_

SELECT '1등', login, Lotto1, Lotto2, Lotto3, Lotto4, Lotto5 FROM dbo.EventLottoP(NOLOCK)
    WHERE Lotto = @iLotto


