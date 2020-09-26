ALTER TABLE [dbo].[CPStatCount] ADD
CONSTRAINT [FK_CPStatCount_CPPromotion] FOREIGN KEY ([Promotion]) REFERENCES [dbo].[CPPromotion] ([Promotion]) ON DELETE CASCADE ON UPDATE CASCADE


