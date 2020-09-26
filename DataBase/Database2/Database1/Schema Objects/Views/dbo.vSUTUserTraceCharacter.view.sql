CREATE VIEW [dbo].[vSUTUserTraceCharacter] AS
select
    LoginUID
,   UID
,   1 as UID2
,   CharType
,   [EXP]
,   Level
,   PlayCount
,   Turtle
from dbo.SUTUserTraceCharacter
with (nolock)


