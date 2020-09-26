CREATE TABLE [dbo].[IndigoOpenTime]
(
[UID] [int] NOT NULL IDENTITY(1, 1),
[DayOfWeek] [smallint] NOT NULL,
[Open] [smalldatetime] NOT NULL,
[Close] [smalldatetime] NOT NULL
) ON [PRIMARY]


