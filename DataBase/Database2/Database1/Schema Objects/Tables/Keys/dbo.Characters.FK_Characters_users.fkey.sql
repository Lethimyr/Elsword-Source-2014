ALTER TABLE [dbo].[Characters] ADD
CONSTRAINT [FK_Characters_users] FOREIGN KEY ([Login]) REFERENCES [dbo].[users] ([Login]) ON DELETE CASCADE ON UPDATE CASCADE


