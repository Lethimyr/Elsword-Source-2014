CREATE TABLE [dbo].[ConnectStatusDB]
(
[NO] [int] NOT NULL IDENTITY(1, 1),
[GameCode] [int] NOT NULL,
[ServerCode] [int] NOT NULL,
[UserNum] [int] NULL,
[ServerIP] [varchar] (25) NULL,
[ServerPort] [int] NULL,
[ServerPart] [smallint] NULL,
[ServerName] [nvarchar] (20) NULL,
[MaxNum] [int] NULL,
[ExtraFlag] [int] NOT NULL
) ON [PRIMARY]


