CREATE TABLE [dbo].[EPEventPerfect]
(
[EventUID] [int] NOT NULL IDENTITY(1, 1),
[StartDate] [smalldatetime] NOT NULL,
[EndDate] [smalldatetime] NOT NULL,
[EventID] [int] NOT NULL,
[GPRatio] [float] NOT NULL,
[EXPRatio] [float] NOT NULL,
[ApplicationType] [bit] NOT NULL,
[EquipItemA] [int] NOT NULL,
[EquipItemB] [int] NOT NULL,
[ActivityRate] [float] NOT NULL,
[CharType] [int] NOT NULL,
[Promotion] [int] NOT NULL,
[Reserved1] [int] NOT NULL,
[Reserved2] [int] NOT NULL,
[Reserved3] [int] NOT NULL,
[Reserved4] [int] NOT NULL,
[Reserved5] [int] NOT NULL,
[Reserved6] [int] NOT NULL,
[Reserved7] [int] NOT NULL,
[Reserved8] [int] NOT NULL,
[Reserved9] [int] NOT NULL
) ON [PRIMARY]


