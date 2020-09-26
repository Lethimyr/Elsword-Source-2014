CREATE TABLE [dbo].[GUser]
(
[UserUID] [bigint] NOT NULL,
[UserID] [nvarchar] (50) NOT NULL,
[USSize] [tinyint] NOT NULL,
[RegDate] [smalldatetime] NOT NULL,
[DelDate] [smalldatetime] NOT NULL,
[Deleted] AS (case [RegDate] when [DelDate] then (0) else (1) end)
) ON [PRIMARY]


