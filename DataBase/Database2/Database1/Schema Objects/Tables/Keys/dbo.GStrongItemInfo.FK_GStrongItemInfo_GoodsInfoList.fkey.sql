ALTER TABLE [dbo].[GStrongItemInfo] ADD
CONSTRAINT [FK_GStrongItemInfo_GoodsInfoList] FOREIGN KEY ([ItemID]) REFERENCES [dbo].[GoodsInfoList] ([GoodsID]) ON DELETE CASCADE ON UPDATE CASCADE


