CREATE TABLE [dbo].[EPGAEventPerfect]
(
[EventUID] [int] NOT NULL,
[EventID] [int] NOT NULL,
[StartDate] [smalldatetime] NOT NULL,
[EndDate] [smalldatetime] NOT NULL,
[ApplicationType] [bit] NOT NULL,
[ActivityRate] [float] NOT NULL,
[EXPRatio] [float] NOT NULL,
[GPRatio] [float] NOT NULL
) ON [PRIMARY]


