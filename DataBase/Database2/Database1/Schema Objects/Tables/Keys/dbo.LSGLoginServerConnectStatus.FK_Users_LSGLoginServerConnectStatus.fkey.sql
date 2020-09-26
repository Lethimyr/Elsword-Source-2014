ALTER TABLE [dbo].[LSGLoginServerConnectStatus] ADD
CONSTRAINT [FK_Users_LSGLoginServerConnectStatus] FOREIGN KEY ([LoginUID]) REFERENCES [dbo].[users] ([LoginUID]) ON DELETE CASCADE ON UPDATE CASCADE


