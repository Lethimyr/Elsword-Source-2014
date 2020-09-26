CREATE PROCEDURE [dbo].[RGR_rank_grandchaser_daily_select_loginuid]
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
from dbo.RGRRankGrandchaserDaily
with (nolock)
where
    LoginUID = @iLoginUID_
and TabType = @iTabType_
option (maxdop 1)


