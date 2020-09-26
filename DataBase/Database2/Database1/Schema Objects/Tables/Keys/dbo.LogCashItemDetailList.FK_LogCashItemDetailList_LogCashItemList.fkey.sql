ALTER TABLE [dbo].[LogCashItemDetailList] ADD
CONSTRAINT [FK_LogCashItemDetailList_LogCashItemList] FOREIGN KEY ([ItemUID]) REFERENCES [dbo].[LogCashItemList] ([ItemUID]) ON DELETE CASCADE ON UPDATE CASCADE


