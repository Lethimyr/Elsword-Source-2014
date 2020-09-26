ALTER TABLE [dbo].[ComicBookItemInfo] ADD
CONSTRAINT [FK_ComicBookItemInfo_ComicBookCouponList] FOREIGN KEY ([SerialNumber]) REFERENCES [dbo].[ComicBookCouponList] ([SerialNumber]) ON DELETE CASCADE ON UPDATE CASCADE


