CREATE TABLE [dbo].[GTutor]
(
[TeacherUID] [bigint] NOT NULL,
[StudentUID] [bigint] NOT NULL,
[RegDate] [smalldatetime] NULL,
[LastDate] [smalldatetime] NULL,
[DelDate] [smalldatetime] NULL,
[Deleted] AS (case [RegDate] when [DelDate] then (0) else (1) end)
) ON [PRIMARY]


