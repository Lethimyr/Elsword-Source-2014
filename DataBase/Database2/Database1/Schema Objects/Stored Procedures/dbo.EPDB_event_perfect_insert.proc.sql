CREATE PROCEDURE [dbo].[EPDB_event_perfect_insert]
	@iEventUID_ [int],
	@iEventID_ [int],
	@sdtStartDate_ [smalldatetime],
	@sdtEndDate_ [smalldatetime],
	@bApplicationType_ [bit],
	@iActivityRate_ [int],
	@fEXPRatio_ [float],
	@fGPRatio_ [float],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
    mk8253.이벤트 목록 추가
*/

begin transaction
    INSERT INTO dbo.EPGAEventPerfect
    with (updlock)
    (
	    EventUID
    ,   EventID
    ,   StartDate
    ,   EndDate
    ,   ApplicationType
    ,   ActivityRate
    ,   EXPRatio
    ,   GPRatio
    )
    select
        @iEventUID_
    ,   @iEventID_
    ,   @sdtStartDate_
    ,   @sdtEndDate_
    ,   @bApplicationType_
    ,   @iActivityRate_
    ,   @fEXPRatio_
    ,   @fGPRatio_
        
    if @@error <> 0
    begin select @iOK = -101 goto fail_tran end
commit transaction

goto end_proc

fail_tran:
    rollback transaction

end_proc:
    select @iOK as OK


