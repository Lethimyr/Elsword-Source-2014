CREATE PROCEDURE [dbo].[RGR_rank_grandchaser_weekly_select_top100]
AS
set nocount on

select
    TabType
,   Rank
,   LoginUID
,   GuildID
,   GuildMark
,   GuildName
,   CharType
,   Nick
,   Win
,   Lose
,   WinRate
from dbo.RGRRankGrandchaserWeekly
with (nolock)
where
    Rank between 1 and 100
option (maxdop 1)


