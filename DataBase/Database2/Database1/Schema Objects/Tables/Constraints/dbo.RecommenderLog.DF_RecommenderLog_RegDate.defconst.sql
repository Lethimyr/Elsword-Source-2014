ALTER TABLE [dbo].[RecommenderLog] ADD CONSTRAINT [DF_RecommenderLog_RegDate] DEFAULT (getdate()) FOR [RegDate]


