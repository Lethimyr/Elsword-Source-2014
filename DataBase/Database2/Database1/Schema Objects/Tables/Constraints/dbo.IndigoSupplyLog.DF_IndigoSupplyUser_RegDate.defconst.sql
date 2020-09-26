ALTER TABLE [dbo].[IndigoSupplyLog] ADD CONSTRAINT [DF_IndigoSupplyUser_RegDate] DEFAULT (getdate()) FOR [RegDate]


