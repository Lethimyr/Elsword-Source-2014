CREATE TABLE [dbo].[StatPCBangLoginCount]
(
[ID] [int] NOT NULL IDENTITY(1, 1),
[Login] [nvarchar] (20) NOT NULL,
[TotalPlayTime] [int] NOT NULL,
[ConnectCount] [int] NOT NULL,
[RegDate] [smalldatetime] NOT NULL
) ON [PRIMARY]


