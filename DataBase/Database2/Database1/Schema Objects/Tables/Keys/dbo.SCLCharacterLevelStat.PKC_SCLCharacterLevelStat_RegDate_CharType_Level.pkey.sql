ALTER TABLE [dbo].[SCLCharacterLevelStat] ADD CONSTRAINT [PKC_SCLCharacterLevelStat_RegDate_CharType_Level] PRIMARY KEY CLUSTERED  ([RegDate], [CharType], [Level]) WITH FILLFACTOR=80 ON [PRIMARY]


