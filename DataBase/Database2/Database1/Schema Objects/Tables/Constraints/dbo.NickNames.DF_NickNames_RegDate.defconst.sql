ALTER TABLE [dbo].[NickNames] ADD CONSTRAINT [DF_NickNames_RegDate] DEFAULT (getdate()) FOR [RegDate]


