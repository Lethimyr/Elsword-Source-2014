ALTER TABLE [dbo].[STGSkillTreeSet] ADD CONSTRAINT [PKC_STGSkillTreeSet_LoginUID_CharType_ETC] PRIMARY KEY CLUSTERED  ([LoginUID], [CharType], [Promotion], [SetID], [SlotID], [SkillID]) WITH FILLFACTOR=80 ON [PRIMARY]


