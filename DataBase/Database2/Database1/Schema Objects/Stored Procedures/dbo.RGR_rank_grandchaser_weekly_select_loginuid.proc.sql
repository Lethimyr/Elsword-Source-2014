CREATE PROCEDURE [dbo].[RGR_rank_grandchaser_weekly_select_loginuid]
	@iLoginUID_ [int],
	@iTabType_ [tinyint] = 0
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
    LoginUID = @iLoginUID_
and TabType = @iTabType_
option (maxdop 1)


