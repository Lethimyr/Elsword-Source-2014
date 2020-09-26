CREATE TABLE [dbo].[ECEventChatDate]
(
[UID] [int] NOT NULL IDENTITY(1, 1),
[StartDate] [smalldatetime] NOT NULL,
[EndDate] [smalldatetime] NOT NULL,
[GPRatio] [float] NOT NULL,
[EXPRatio] [float] NOT NULL
) ON [PRIMARY]


