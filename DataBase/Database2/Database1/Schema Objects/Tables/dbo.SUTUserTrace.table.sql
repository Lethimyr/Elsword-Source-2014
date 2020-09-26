CREATE TABLE [dbo].[SUTUserTrace]
(
[LoginUID] [int] NOT NULL,
[UID] [int] NOT NULL IDENTITY(1, 1),
[StartDate] [smalldatetime] NOT NULL,
[EndDate] [smalldatetime] NOT NULL,
[Grade] [tinyint] NOT NULL,
[PlayTime] [int] NOT NULL,
[PCBang] [bit] NOT NULL,
[GuildServer] [bit] NOT NULL
) ON [PRIMARY]


