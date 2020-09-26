ALTER TABLE [dbo].[guild_log] ADD CONSTRAINT [DF_guild_log_regdate] DEFAULT (getdate()) FOR [regdate]


