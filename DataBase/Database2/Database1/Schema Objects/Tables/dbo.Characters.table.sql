CREATE TABLE [dbo].[Characters]
(
[Login] [nvarchar] (20) NOT NULL,
[CharType] [tinyint] NOT NULL,
[Promotion] [tinyint] NOT NULL,
[Exp] [bigint] NOT NULL,
[Level] [int] NOT NULL,
[Win] [int] NOT NULL,
[Lose] [int] NOT NULL
) ON [PRIMARY]


