ALTER TABLE [dbo].[GPost_Attribute] ADD
CONSTRAINT [FK_GPost_Attribute_GPost_PostNo] FOREIGN KEY ([PostNo]) REFERENCES [dbo].[GPost] ([PostNo]) ON DELETE CASCADE ON UPDATE CASCADE


