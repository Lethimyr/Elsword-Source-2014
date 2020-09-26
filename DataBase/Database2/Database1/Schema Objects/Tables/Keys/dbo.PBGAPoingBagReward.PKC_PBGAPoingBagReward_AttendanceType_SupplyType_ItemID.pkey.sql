ALTER TABLE [dbo].[PBGAPoingBagReward] ADD CONSTRAINT [PKC_PBGAPoingBagReward_AttendanceType_SupplyType_ItemID] PRIMARY KEY CLUSTERED  ([AttendanceType], [SupplyType], [ItemID]) WITH FILLFACTOR=80 ON [PRIMARY]


