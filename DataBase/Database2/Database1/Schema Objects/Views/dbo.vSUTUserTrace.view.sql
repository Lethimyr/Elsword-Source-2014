CREATE VIEW [dbo].[vSUTUserTrace] AS
select
    LoginUID
,   UID
,   1 as UID2
,   StartDate
,   EndDate
,   Grade
,   PlayTime
,   PCBang
,   GuildServer
from dbo.SUTUserTrace
with (nolock)


