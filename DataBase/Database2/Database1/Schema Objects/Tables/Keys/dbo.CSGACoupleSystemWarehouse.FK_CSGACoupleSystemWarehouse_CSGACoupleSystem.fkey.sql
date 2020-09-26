ALTER TABLE [dbo].[CSGACoupleSystemWarehouse] ADD
CONSTRAINT [FK_CSGACoupleSystemWarehouse_CSGACoupleSystem] FOREIGN KEY ([LoginUID]) REFERENCES [dbo].[CSGACoupleSystem] ([LoginUID]) ON DELETE CASCADE ON UPDATE CASCADE


