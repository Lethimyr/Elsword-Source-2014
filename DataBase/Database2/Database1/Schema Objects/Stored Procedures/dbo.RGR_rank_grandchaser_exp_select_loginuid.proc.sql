CREATE PROCEDURE [dbo].[RGR_rank_grandchaser_exp_select_loginuid]
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
,   [Exp]
from dbo.RGRRankGrandchaserExp
with (nolock)
where
    LoginUID = @iLoginUID_
and TabType = @iTabType_
option (maxdop 1)


