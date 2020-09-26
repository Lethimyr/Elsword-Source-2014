ALTER TABLE [dbo].[GDenyOption] ADD
CONSTRAINT [FK_GDenyOption_GDenyCode] FOREIGN KEY ([QuestionNo], [CodeNo]) REFERENCES [dbo].[GDenyCode] ([QuestionNo], [CodeNo]) ON DELETE CASCADE ON UPDATE CASCADE


