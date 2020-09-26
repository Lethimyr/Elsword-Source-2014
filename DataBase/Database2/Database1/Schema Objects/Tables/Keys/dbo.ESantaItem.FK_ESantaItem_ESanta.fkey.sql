ALTER TABLE [dbo].[ESantaItem] ADD
CONSTRAINT [FK_ESantaItem_ESanta] FOREIGN KEY ([EventUID], [UserType]) REFERENCES [dbo].[ESanta] ([EventUID], [UserType]) ON DELETE CASCADE ON UPDATE CASCADE


