ALTER TABLE [dbo].[GPetLog] ADD CONSTRAINT [DF_GPetLog_DelDate] DEFAULT (getdate()) FOR [DelDate]


