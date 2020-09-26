ALTER TABLE [dbo].[GuildInfo] ADD CONSTRAINT [DF_GUILDINFO_RegDate] DEFAULT (getdate()) FOR [RegDate]


