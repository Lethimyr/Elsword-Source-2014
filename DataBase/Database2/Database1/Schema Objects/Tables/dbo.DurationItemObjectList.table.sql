CREATE TABLE [dbo].[DurationItemObjectList]
(
[DurationItemUID] [int] NOT NULL IDENTITY(0, 1),
[OwnerLogin] [nvarchar] (20) NOT NULL,
[BuyerLogin] [nvarchar] (20) NOT NULL,
[GoodsID] [int] NOT NULL,
[Duration] [int] NOT NULL,
[RegDate] [smalldatetime] NOT NULL
) ON [PRIMARY]


