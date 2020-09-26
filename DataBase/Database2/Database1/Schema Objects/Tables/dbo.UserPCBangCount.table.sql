CREATE TABLE [dbo].[UserPCBangCount]
(
[ID] [int] NOT NULL IDENTITY(1, 1),
[Login] [nvarchar] (20) NOT NULL,
[IP] [nvarchar] (20) NOT NULL,
[PlayTime] [int] NOT NULL,
[ConnectTime] [smalldatetime] NOT NULL,
[DisconnectTime] [smalldatetime] NOT NULL
) ON [PRIMARY]


