ALTER TABLE [dbo].[GMSMissionSystemSub] ADD CONSTRAINT [PKC_GMSMissionSystemSub_LoginUID_MainMissionID_SubMissionID] PRIMARY KEY CLUSTERED  ([LoginUID], [MainMissionID], [SubMissionID]) WITH FILLFACTOR=80 ON [PRIMARY]


