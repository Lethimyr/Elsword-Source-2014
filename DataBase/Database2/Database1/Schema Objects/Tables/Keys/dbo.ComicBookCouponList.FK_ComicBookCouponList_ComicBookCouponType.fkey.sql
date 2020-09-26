ALTER TABLE [dbo].[ComicBookCouponList] ADD
CONSTRAINT [FK_ComicBookCouponList_ComicBookCouponType] FOREIGN KEY ([CouponType]) REFERENCES [dbo].[ComicBookCouponType] ([CouponType]) ON DELETE CASCADE ON UPDATE CASCADE


