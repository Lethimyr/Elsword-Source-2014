CREATE PROCEDURE [dbo].[ecup_event_chat_date_get]
AS
set nocount on
set transaction isolation level read uncommitted

declare
    @sdtDate smalldatetime
select
    @sdtDate = getdate()

select
    UID
,   StartDate
,   EndDate
,   GPRatio
,   EXPRatio
from dbo.ECEventChatDate
where
    StartDate <= @sdtDate
and @sdtDate < EndDate


