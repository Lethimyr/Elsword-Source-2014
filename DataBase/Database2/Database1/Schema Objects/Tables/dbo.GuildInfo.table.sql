CREATE TABLE [dbo].[GuildInfo]
(
[GuildId] [int] NOT NULL IDENTITY(1, 1),
[Name] [nvarchar] (12) NOT NULL,
[LoadLogin] [nvarchar] (20) NOT NULL,
[Explanation] [nvarchar] (500) NOT NULL,
[Mark] [int] NOT NULL,
[RegDate] [smalldatetime] NOT NULL,
[Point] [int] NOT NULL,
[NumMembers] [smallint] NOT NULL,
[NumMatches] [int] NOT NULL,
[NumWins] [int] NOT NULL,
[NumLooses] [int] NOT NULL,
[Approval] [bit] NOT NULL,
[Email] [nvarchar] (50) NOT NULL,
[cafe_url] [varchar] (20) NULL
) ON [PRIMARY]


