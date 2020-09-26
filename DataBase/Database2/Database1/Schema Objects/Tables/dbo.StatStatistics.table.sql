CREATE TABLE [dbo].[StatStatistics]
(
[RegDate] [smalldatetime] NOT NULL,
[MaxTime] [smalldatetime] NOT NULL,
[DailyMaxUsers] [int] NOT NULL,
[TotalUsers] [int] NOT NULL,
[DailyNewUsers] [int] NOT NULL,
[WeeklyNewUsers] [int] NOT NULL,
[MonthlyNewUsers] [int] NOT NULL,
[DailyUniqueUsers] [int] NOT NULL,
[WeeklyUniqueUsers] [int] NOT NULL,
[MonthlyUniqueUsers] [int] NOT NULL,
[DailyAvgUsers] [int] NOT NULL,
[WeeklyAvgUsers] [int] NOT NULL,
[MonthlyAvgUsers] [int] NOT NULL,
[TotalUserCount] [int] NOT NULL,
[DailyNewUserCount] [int] NOT NULL,
[WeeklyNewUserCount] [int] NOT NULL,
[MonthlyNewUserCount] [int] NOT NULL,
[DailyUniqueUserCount] [int] NOT NULL,
[WeeklyUniqueUserCount] [int] NOT NULL,
[MonthlyUniqueUserCount] [int] NOT NULL
) ON [PRIMARY]


