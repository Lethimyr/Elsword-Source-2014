ALTER TABLE [dbo].[CashItemDetailList] ADD
CONSTRAINT [FK_CashItemDetailList_CashItemList] FOREIGN KEY ([ItemUID]) REFERENCES [dbo].[CashItemList] ([ItemUID]) ON DELETE CASCADE ON UPDATE CASCADE


