CREATE TABLE [dbo].[Notice]
(
[GuildId] [int] NOT NULL,
[MsgId] [int] NOT NULL,
[Msg] [nvarchar] (250) NOT NULL,
[Valid] [bit] NOT NULL,
[RegDate] [datetime] NOT NULL
) ON [PRIMARY]


