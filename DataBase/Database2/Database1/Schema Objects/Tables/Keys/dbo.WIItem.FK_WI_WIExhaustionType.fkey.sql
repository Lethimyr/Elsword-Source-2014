ALTER TABLE [dbo].[WIItem] ADD
CONSTRAINT [FK_WI_WIExhaustionType] FOREIGN KEY ([ExhaustionType]) REFERENCES [dbo].[WIExhaustionType] ([ExhaustionType]) ON DELETE CASCADE ON UPDATE CASCADE


