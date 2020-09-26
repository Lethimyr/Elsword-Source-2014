CREATE TABLE [dbo].[IndigoSupplyItemByRank]
(
[UID] [int] NOT NULL IDENTITY(1, 1),
[CharType] [tinyint] NOT NULL,
[StartRank] [int] NOT NULL,
[EndRank] [int] NOT NULL,
[SupplyType] [int] NOT NULL
) ON [PRIMARY]


