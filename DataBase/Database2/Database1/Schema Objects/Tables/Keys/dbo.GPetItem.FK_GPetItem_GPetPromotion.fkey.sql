ALTER TABLE [dbo].[GPetItem] ADD
CONSTRAINT [FK_GPetItem_GPetPromotion] FOREIGN KEY ([Promotion]) REFERENCES [dbo].[GPetPromotion] ([Promotion]) ON DELETE CASCADE ON UPDATE CASCADE


