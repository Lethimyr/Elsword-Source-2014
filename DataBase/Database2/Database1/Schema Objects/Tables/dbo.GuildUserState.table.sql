CREATE TABLE [dbo].[GuildUserState]
(
[NO] [int] NOT NULL IDENTITY(1, 1),
[ID] [nvarchar] (20) NOT NULL,
[CN] [varchar] (12) NULL,
[GuildID] [int] NULL,
[Contents] [text] NOT NULL,
[Character] [tinyint] NULL,
[Level] [int] NULL,
[Curl] [nvarchar] (20) NOT NULL,
[RegDate] [datetime] NULL,
[State] [tinyint] NULL
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]


