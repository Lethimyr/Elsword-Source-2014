ALTER TABLE [dbo].[ECEventChatDate] ADD CONSTRAINT [UK_ECEventChatDate_StartDate_EndDate] UNIQUE CLUSTERED  ([StartDate], [EndDate]) WITH FILLFACTOR=80 ON [PRIMARY]


