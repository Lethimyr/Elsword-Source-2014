CREATE TABLE [dbo].[GItem]
(
[ItemUID] [bigint] NOT NULL IDENTITY(1, 1),
[UnitUID] [bigint] NOT NULL,
[ItemID] [int] NOT NULL,
[InventoryCategory] [tinyint] NOT NULL,
[SlotID] [tinyint] NOT NULL,
[RegDate] [smalldatetime] NOT NULL,
[DelDate] [smalldatetime] NOT NULL,
[Deleted] AS (case [RegDate] when [DelDate] then (0) else (1) end)
) ON [PRIMARY]


