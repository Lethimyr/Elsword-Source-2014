CREATE TABLE [dbo].[StatPCBangIPCount]
(
[ID] [int] NOT NULL IDENTITY(1, 1),
[IP] [nvarchar] (20) NOT NULL,
[TotalPlayTime] [int] NOT NULL,
[ConnectCount] [int] NOT NULL,
[UserCount] [int] NOT NULL,
[RegDate] [smalldatetime] NOT NULL
) ON [PRIMARY]


