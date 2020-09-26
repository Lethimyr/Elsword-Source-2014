CREATE TABLE [dbo].[CSGACheckSum]
(
[Type] [tinyint] NOT NULL,
[Protocol] [int] NOT NULL,
[CheckSum] [binary] (4) NOT NULL,
[RegDateA] [datetime] NOT NULL
) ON [PRIMARY]


