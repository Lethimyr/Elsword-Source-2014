ALTER TABLE [dbo].[DelGuildUser] ADD CONSTRAINT [DF_DelGuildUser_RegDate] DEFAULT (getdate()) FOR [RegDate]


