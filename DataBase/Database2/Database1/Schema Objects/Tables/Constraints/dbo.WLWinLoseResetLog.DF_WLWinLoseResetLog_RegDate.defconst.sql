ALTER TABLE [dbo].[WLWinLoseResetLog] ADD CONSTRAINT [DF_WLWinLoseResetLog_RegDate] DEFAULT (getdate()) FOR [RegDate]


