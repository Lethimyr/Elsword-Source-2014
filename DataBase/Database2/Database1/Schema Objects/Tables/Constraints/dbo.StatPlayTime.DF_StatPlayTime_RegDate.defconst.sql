ALTER TABLE [dbo].[StatPlayTime] ADD CONSTRAINT [DF_StatPlayTime_RegDate] DEFAULT (getdate()) FOR [RegDate]


