CREATE TABLE [dbo].[LogCashItemList]
(
[ItemUID] [int] NOT NULL,
[OwnerLoginUID] [int] NOT NULL,
[BuyerLoginUID] [int] NOT NULL,
[Comment] [nvarchar] (80) NULL,
[BuyDate] [smalldatetime] NOT NULL,
[SupplyDate] [smalldatetime] NOT NULL
) ON [PRIMARY]


