ALTER TABLE [dbo].[tbl_Event_SnowCoinCollected] ADD CONSTRAINT [DF_tblChaseSnowCoin_Regdate] DEFAULT (getdate()) FOR [Regdate]


