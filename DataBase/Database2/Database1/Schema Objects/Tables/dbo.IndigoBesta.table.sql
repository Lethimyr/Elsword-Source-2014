CREATE TABLE [dbo].[IndigoBesta]
(
[UID] [int] NOT NULL IDENTITY(1, 1),
[RegDate] [smalldatetime] NOT NULL,
[Login] [nvarchar] (20) NOT NULL,
[Nick] [nvarchar] (24) NOT NULL,
[RP] [int] NOT NULL,
[Win] [int] NOT NULL,
[Lose] [int] NOT NULL,
[WinRate] [real] NOT NULL
) ON [PRIMARY]


