ALTER TABLE [dbo].[TCGTitleCollectionSub] ADD CONSTRAINT [PKC_TCGTitleCollectionSub_LoginUID_MainMissionID_SubMissionID] PRIMARY KEY CLUSTERED  ([LoginUID], [MainMissionID], [SubMissionID]) WITH FILLFACTOR=80 ON [PRIMARY]


