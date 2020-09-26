CREATE TABLE [dbo].[CashItemList]
(
[ItemUID] [int] NOT NULL IDENTITY(1, 1),
[OwnerLoginUID] [int] NOT NULL,
[BuyerLoginUID] [int] NOT NULL,
[Comment] [nvarchar] (80) NULL,
[RegDate] [smalldatetime] NOT NULL
) ON [PRIMARY]


