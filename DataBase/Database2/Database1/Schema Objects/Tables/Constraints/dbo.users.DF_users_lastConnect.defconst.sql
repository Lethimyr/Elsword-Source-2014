ALTER TABLE [dbo].[users] ADD CONSTRAINT [DF_users_lastConnect] DEFAULT (getdate()) FOR [lastConnect]


