CREATE TABLE [dbo].[GoodsObjectList]
(
[ItemUID] [int] NOT NULL IDENTITY(0, 1),
[OwnerLogin] [nvarchar] (20) NOT NULL,
[BuyerLogin] [nvarchar] (20) NOT NULL,
[ItemID] [int] NOT NULL,
[RegDate] [smalldatetime] NOT NULL,
[StartDate] [smalldatetime] NOT NULL,
[EndDate] [smalldatetime] NOT NULL,
[Period] [int] NOT NULL
) ON [PRIMARY]


