CREATE PROCEDURE [dbo].[RGR_rank_grandchaser_exp_select_top100]
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
    Rank between 1 and 100
option (maxdop 1)


