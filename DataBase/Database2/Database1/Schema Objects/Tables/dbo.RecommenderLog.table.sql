CREATE TABLE [dbo].[RecommenderLog]
(
[LoginUID] [int] NOT NULL,
[ItemUID] [int] NOT NULL,
[ItemID] [int] NOT NULL,
[ItemType] [int] NOT NULL,
[Factor] [int] NOT NULL,
[Price] [int] NOT NULL,
[RegDate] [smalldatetime] NOT NULL
) ON [PRIMARY]


