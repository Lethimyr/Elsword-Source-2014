CREATE TABLE [dbo].[Rank_]
(
[unitUID] [bigint] NULL,
[NickName] [nvarchar] (16) NOT NULL,
[VsRank] [bigint] NOT NULL IDENTITY(1, 1),
[ExpRank] [bigint] NOT NULL,
[win] [int] NOT NULL,
[lose] [int] NOT NULL,
[exp] [int] NOT NULL
) ON [PRIMARY]


