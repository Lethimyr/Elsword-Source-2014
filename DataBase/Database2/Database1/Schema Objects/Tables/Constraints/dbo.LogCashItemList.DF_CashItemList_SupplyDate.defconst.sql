ALTER TABLE [dbo].[LogCashItemList] ADD CONSTRAINT [DF_CashItemList_SupplyDate] DEFAULT (getdate()) FOR [SupplyDate]


