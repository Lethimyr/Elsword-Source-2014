CREATE TABLE [dbo].[RGRRankGrandchaserDaily_old]
(
[TabType] [tinyint] NOT NULL,
[Rank] [int] NOT NULL,
[LoginUID] [int] NOT NULL,
[GuildID] [int] NOT NULL,
[GuildMark] [int] NOT NULL,
[GuildName] [nvarchar] (12) NOT NULL,
[CharType] [tinyint] NOT NULL,
[Nick] [nvarchar] (24) NOT NULL,
[Win] [int] NOT NULL,
[Lose] [int] NOT NULL,
[WinRate] AS (case when ([Win] > 0) then (([Win] + 0.0) / ([Win] + [Lose] + 0.0) * 100) else 0.0 end)
) ON [PRIMARY]


