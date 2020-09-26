CREATE TABLE [dbo].[NMWNetMarbleGoodsInfoList]
(
[GoodsID] [int] NOT NULL,
[GoodsName] [nvarchar] (50) NOT NULL,
[Description] [nvarchar] (200) NOT NULL,
[IsLimited] [bit] NOT NULL,
[Duration] [int] NOT NULL,
[CharType] [smallint] NOT NULL,
[Kind] [smallint] NOT NULL,
[ItemType] [int] NOT NULL,
[ShowWeb] [bit] NOT NULL,
[HotCount] [int] NOT NULL,
[Price] [int] NOT NULL,
[IsNewGoods] [bit] NOT NULL,
[Cash] [bit] NOT NULL,
[GoodsClass] [smallint] NOT NULL,
[Img] [nvarchar] (50) NOT NULL,
[RegDate] [smalldatetime] NOT NULL,
[IsSubSetGoods] [bit] NOT NULL,
[SetGoodsNum] [int] NOT NULL,
[Recom] [int] NOT NULL,
[Category] [int] NOT NULL,
[Attack] [int] NOT NULL,
[Defence] [int] NOT NULL,
[HPInc] [int] NOT NULL,
[LastingPrice] [int] NOT NULL
) ON [PRIMARY]


