ALTER TABLE [dbo].[LogNickNames] ADD CONSTRAINT [DF_LogNickNames_RegDate] DEFAULT (getdate()) FOR [RegDate]


