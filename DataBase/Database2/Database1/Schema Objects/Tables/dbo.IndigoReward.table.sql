CREATE TABLE [dbo].[IndigoReward]
(
[UID] [int] NOT NULL IDENTITY(1, 1),
[Rank] [int] NOT NULL,
[ItemID] [int] NOT NULL,
[CharType] [int] NOT NULL,
[ItemName] [nvarchar] (100) NOT NULL,
[ItemType] [int] NOT NULL,
[Duration] [int] NOT NULL
) ON [PRIMARY]


