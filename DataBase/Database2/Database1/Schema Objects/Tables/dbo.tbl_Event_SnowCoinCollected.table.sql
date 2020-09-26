CREATE TABLE [dbo].[tbl_Event_SnowCoinCollected]
(
[tm_num] [int] NOT NULL IDENTITY(1, 1),
[UniID] [char] (12) NOT NULL,
[UserID] [nvarchar] (12) NOT NULL,
[tc_num] [int] NULL,
[Regdate] [smalldatetime] NOT NULL,
[IsUse] [char] (1) NOT NULL
) ON [PRIMARY]


