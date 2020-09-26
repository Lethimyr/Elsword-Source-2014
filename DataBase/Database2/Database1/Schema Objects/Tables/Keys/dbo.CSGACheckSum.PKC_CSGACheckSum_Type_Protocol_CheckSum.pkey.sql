ALTER TABLE [dbo].[CSGACheckSum] ADD CONSTRAINT [PKC_CSGACheckSum_Type_Protocol_CheckSum] PRIMARY KEY CLUSTERED  ([Type], [Protocol], [CheckSum]) WITH FILLFACTOR=80 ON [PRIMARY]


