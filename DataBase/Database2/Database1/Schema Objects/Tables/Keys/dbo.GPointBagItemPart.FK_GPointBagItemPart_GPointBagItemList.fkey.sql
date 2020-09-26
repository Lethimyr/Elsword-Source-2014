ALTER TABLE [dbo].[GPointBagItemPart] ADD
CONSTRAINT [FK_GPointBagItemPart_GPointBagItemList] FOREIGN KEY ([ItemID]) REFERENCES [dbo].[GPointBagItemList] ([ItemID]) ON DELETE CASCADE ON UPDATE CASCADE


