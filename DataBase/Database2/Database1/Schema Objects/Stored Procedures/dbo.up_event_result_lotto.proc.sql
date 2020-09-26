CREATE PROCEDURE [dbo].[up_event_result_lotto]
	@iRound_ [int]
AS
exec dbo.up_log_set_proc_count 'up_event_result_lotto'

SET NOCOUNT ON

SELECT Lotto1, Lotto2, Lotto3, Lotto4, Lotto5 FROM dbo.EventLottoR(NOLOCK) WHERE [Round] = @iRound_


