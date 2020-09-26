ALTER TABLE [dbo].[GMSMissionSystemSub] ADD
CONSTRAINT [FK_GMSMissionSystemSub_GMSMissionSystemMain] FOREIGN KEY ([LoginUID], [MainMissionID]) REFERENCES [dbo].[GMSMissionSystemMain] ([LoginUID], [MainMissionID]) ON DELETE CASCADE ON UPDATE CASCADE


