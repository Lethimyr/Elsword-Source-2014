CREATE TABLE [dbo].[TCGTitleCollectionMain]
(
[LoginUID] [int] NOT NULL,
[MainMissionID] [int] NOT NULL,
[StartDate] [smalldatetime] NOT NULL,
[EndDate] [smalldatetime] NOT NULL,
[Status] AS (case [StartDate] when [EndDate] then 1 else 2 end)
) ON [PRIMARY]


