ALTER TABLE [dbo].[CPDailyCount] ADD
CONSTRAINT [FK_CPDailyCount_CPPromotion] FOREIGN KEY ([Promotion]) REFERENCES [dbo].[CPPromotion] ([Promotion]) ON DELETE CASCADE ON UPDATE CASCADE


