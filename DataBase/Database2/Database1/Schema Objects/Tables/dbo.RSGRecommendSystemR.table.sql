CREATE TABLE [dbo].[RSGRecommendSystemR]
(
[LoginUID] [int] NOT NULL,
[UserType] [tinyint] NOT NULL,
[CheckData] [int] NOT NULL,
[ELoginUID] [int] NOT NULL,
[RegDate] [smalldatetime] NOT NULL,
[RecomDate] [smalldatetime] NOT NULL,
[SupDate] [smalldatetime] NOT NULL,
[Recommended] AS (case [RegDate] when [RecomDate] then 0 else 1 end),
[Supplied] AS (case [RegDate] when [SupDate] then 0 else 1 end)
) ON [PRIMARY]


