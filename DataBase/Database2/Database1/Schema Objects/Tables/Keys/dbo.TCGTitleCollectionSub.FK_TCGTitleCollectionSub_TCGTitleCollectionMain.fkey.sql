ALTER TABLE [dbo].[TCGTitleCollectionSub] ADD
CONSTRAINT [FK_TCGTitleCollectionSub_TCGTitleCollectionMain] FOREIGN KEY ([LoginUID], [MainMissionID]) REFERENCES [dbo].[TCGTitleCollectionMain] ([LoginUID], [MainMissionID]) ON DELETE CASCADE ON UPDATE CASCADE


