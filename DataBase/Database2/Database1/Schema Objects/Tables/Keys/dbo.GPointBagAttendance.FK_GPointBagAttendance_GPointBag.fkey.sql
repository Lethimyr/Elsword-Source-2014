ALTER TABLE [dbo].[GPointBagAttendance] ADD
CONSTRAINT [FK_GPointBagAttendance_GPointBag] FOREIGN KEY ([LoginUID]) REFERENCES [dbo].[GPointBag] ([LoginUID]) ON DELETE CASCADE ON UPDATE CASCADE


