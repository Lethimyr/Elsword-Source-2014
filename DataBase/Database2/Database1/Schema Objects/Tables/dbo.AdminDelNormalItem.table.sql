CREATE TABLE [dbo].[AdminDelNormalItem]
(
[GoodsUID] [int] NOT NULL IDENTITY(0, 1),
[OwnerLogin] [nvarchar] (20) NOT NULL,
[BuyerLogin] [nvarchar] (20) NOT NULL,
[GoodsID] [int] NOT NULL,
[RegDate] [smalldatetime] NOT NULL,
[DelDate] [smalldatetime] NOT NULL,
[DelLogin] [nvarchar] (20) NOT NULL
) ON [PRIMARY]


