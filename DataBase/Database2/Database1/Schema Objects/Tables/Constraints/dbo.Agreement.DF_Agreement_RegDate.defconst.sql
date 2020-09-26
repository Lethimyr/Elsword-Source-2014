ALTER TABLE [dbo].[Agreement] ADD CONSTRAINT [DF_Agreement_RegDate] DEFAULT (getdate()) FOR [RegDate]


