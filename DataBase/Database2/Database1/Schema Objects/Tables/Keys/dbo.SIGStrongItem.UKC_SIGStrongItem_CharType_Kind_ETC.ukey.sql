ALTER TABLE [dbo].[SIGStrongItem] ADD CONSTRAINT [UKC_SIGStrongItem_CharType_Kind_ETC] UNIQUE CLUSTERED  ([CharType], [Kind], [Level], [UnableStrong]) WITH FILLFACTOR=80 ON [PRIMARY]


