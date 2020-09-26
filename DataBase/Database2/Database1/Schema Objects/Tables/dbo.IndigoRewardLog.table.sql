CREATE TABLE [dbo].[IndigoRewardLog]
(
[Login] [nvarchar] (20) NOT NULL,
[Rank] [int] NOT NULL,
[ItemID] [int] NOT NULL,
[CharType] [int] NOT NULL,
[ItemName] [nvarchar] (100) NOT NULL,
[Duration] [int] NOT NULL,
[RecvDate] [smalldatetime] NOT NULL,
[OK] [int] NOT NULL
) ON [PRIMARY]


