ALTER TABLE [dbo].[users] ADD CONSTRAINT [DF_users_lastLogin] DEFAULT (getdate()) FOR [lastLogin]


