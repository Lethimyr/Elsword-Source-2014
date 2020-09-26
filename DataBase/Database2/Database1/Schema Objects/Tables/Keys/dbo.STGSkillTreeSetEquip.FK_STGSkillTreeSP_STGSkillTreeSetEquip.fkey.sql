ALTER TABLE [dbo].[STGSkillTreeSetEquip] ADD
CONSTRAINT [FK_STGSkillTreeSP_STGSkillTreeSetEquip] FOREIGN KEY ([LoginUID], [CharType]) REFERENCES [dbo].[STGSkillTreeSP] ([LoginUID], [CharType]) ON DELETE CASCADE ON UPDATE CASCADE


