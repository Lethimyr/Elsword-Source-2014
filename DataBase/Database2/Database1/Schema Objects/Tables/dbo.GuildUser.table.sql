CREATE TABLE [dbo].[GuildUser]
(
[Login] [nvarchar] (20) NOT NULL,
[GuildId] [int] NOT NULL,
[RegDate] [smalldatetime] NOT NULL,
[Point] [int] NOT NULL,
[UserLevel] [smallint] NOT NULL
) ON [PRIMARY]


