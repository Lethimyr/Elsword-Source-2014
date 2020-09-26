CREATE TABLE [dbo].[GPost]
(
[PostNo] [bigint] NOT NULL IDENTITY(1, 1),
[FromUnitUID] [bigint] NOT NULL,
[ToUnitUID] [bigint] NOT NULL,
[Quantity] [int] NOT NULL,
[ScType] [tinyint] NOT NULL,
[ScIndex] [bigint] NOT NULL,
[RegDate] [smalldatetime] NULL,
[DelDate] [smalldatetime] NULL,
[IsReceive] [bit] NULL,
[IsRead] [bit] NULL,
[Deleted] AS (case [RegDate] when [DelDate] then (0) else (1) end),
[Title] [nvarchar] (38) NULL,
[Message] [nvarchar] (600) NULL
) ON [PRIMARY]


