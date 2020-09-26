ALTER TABLE [dbo].[CSGACoupleSystemCharacter] ADD
CONSTRAINT [FK_CSGACoupleSystemCharacter_CSGACoupleSystem] FOREIGN KEY ([LoginUID]) REFERENCES [dbo].[CSGACoupleSystem] ([LoginUID]) ON DELETE CASCADE ON UPDATE CASCADE


