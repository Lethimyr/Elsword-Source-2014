CREATE TABLE [dbo].[IndigoRank]
(
[Rank] [int] NOT NULL IDENTITY(1, 1),
[Login] [nvarchar] (20) NOT NULL,
[Nick] [nvarchar] (24) NOT NULL,
[LoginUID] [int] NOT NULL,
[RP] [int] NOT NULL,
[Win] [int] NOT NULL,
[Lose] [int] NOT NULL,
[WinRate] [real] NOT NULL,
[Change] [int] NOT NULL
) ON [PRIMARY]


