ALTER TABLE [dbo].[users] ADD CONSTRAINT [DF_users_firstLogin] DEFAULT (getdate()) FOR [firstLogin]


