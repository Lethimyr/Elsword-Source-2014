ALTER TABLE [dbo].[ClientChecksum] ADD CONSTRAINT [DF_ClientChecksum_RegDate] DEFAULT (getdate()) FOR [RegDate]


