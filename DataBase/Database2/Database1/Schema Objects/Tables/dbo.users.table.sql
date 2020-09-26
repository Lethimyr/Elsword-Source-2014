CREATE TABLE [dbo].[users]
(
[Login] [nvarchar] (20) NOT NULL,
[passwd] [nvarchar] (32) NOT NULL,
[sex] [char] (1) NOT NULL,
[LoginUID] [int] NOT NULL IDENTITY(1, 1),
[firstLogin] [smalldatetime] NOT NULL,
[lastConnect] [smalldatetime] NOT NULL,
[lastLogin] [smalldatetime] NOT NULL,
[playTime] [int] NOT NULL,
[gamePoint] [int] NOT NULL,
[IPAddress] [nvarchar] (15) NOT NULL,
[Connecting] [bit] NOT NULL,
[ModeLevel] [varbinary] (200) NOT NULL,
[Grade] [int] NOT NULL
) ON [PRIMARY]


