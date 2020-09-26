ALTER TABLE [dbo].[GDenyOption] ADD
CONSTRAINT [FK_GDenyOption_GDenyOption] FOREIGN KEY ([UnitUID], [QuestionNo]) REFERENCES [dbo].[GDenyOption] ([UnitUID], [QuestionNo])


