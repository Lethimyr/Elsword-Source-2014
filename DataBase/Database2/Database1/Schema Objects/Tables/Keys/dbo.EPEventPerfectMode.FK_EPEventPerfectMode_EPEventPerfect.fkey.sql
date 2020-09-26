ALTER TABLE [dbo].[EPEventPerfectMode] ADD
CONSTRAINT [FK_EPEventPerfectMode_EPEventPerfect] FOREIGN KEY ([EventUID]) REFERENCES [dbo].[EPEventPerfect] ([EventUID]) ON DELETE CASCADE ON UPDATE CASCADE


