CREATE TABLE [dbo].[guild_log]
(
[seq] [int] NOT NULL IDENTITY(1, 1),
[part] [int] NOT NULL,
[comment] [varchar] (2000) NULL,
[beforegame] [varchar] (3) NULL,
[aftergame] [varchar] (3) NULL,
[afterclub] [varchar] (3) NULL,
[regdate] [datetime] NULL,
[gamertncode] [varchar] (3) NULL,
[clubrtncode] [varchar] (3) NULL
) ON [PRIMARY]


