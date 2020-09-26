ALTER TABLE [dbo].[GuildUser] ADD CONSTRAINT [DF_GUILDUSER_RegDate] DEFAULT (getdate()) FOR [RegDate]


