CREATE TABLE [dbo].[CashItemDisplayList]
(
[ItemUID] [int] NOT NULL,
[BuyerLoginUID] [int] NOT NULL,
[OwnerLoginUID] [int] NOT NULL,
[ItemID] [int] NOT NULL,
[Factor] [int] NOT NULL,
[Price] [int] NOT NULL,
[FromWeb] [tinyint] NOT NULL,
[RegDate] [smalldatetime] NOT NULL
) ON [PRIMARY]


