ALTER TABLE [dbo].[GMSMissionSystemUniqueLog] ADD
CONSTRAINT [FK_GMSMissionSystemUniqueLog_Users] FOREIGN KEY ([LoginUID]) REFERENCES [dbo].[users] ([LoginUID]) ON DELETE CASCADE ON UPDATE CASCADE


