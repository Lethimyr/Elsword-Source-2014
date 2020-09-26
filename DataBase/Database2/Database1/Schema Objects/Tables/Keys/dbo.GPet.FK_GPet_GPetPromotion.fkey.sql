ALTER TABLE [dbo].[GPet] ADD
CONSTRAINT [FK_GPet_GPetPromotion] FOREIGN KEY ([Promotion]) REFERENCES [dbo].[GPetPromotion] ([Promotion]) ON DELETE CASCADE ON UPDATE CASCADE


