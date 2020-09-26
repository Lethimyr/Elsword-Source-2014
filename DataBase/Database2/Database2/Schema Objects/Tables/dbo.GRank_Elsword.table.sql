CREATE TABLE [dbo].[GRank_Elsword]
(
[unitUID] [bigint] NOT NULL,
[NickName] [nvarchar] (16) NOT NULL,
[VsRank] [bigint] NOT NULL IDENTITY(1, 1),
[ExpRank] [bigint] NOT NULL,
[Win] [bigint] NOT NULL,
[Lose] [bigint] NOT NULL,
[Level] [smallint] NOT NULL,
[Exp] [bigint] NOT NULL
) ON [PRIMARY]


