CREATE TABLE [dbo].[GStrongItemLog]
(
[LoginUID] [int] NOT NULL,
[ItemUID] [int] NOT NULL,
[ItemID] [int] NOT NULL,
[StrongLevel1] [tinyint] NOT NULL,
[StrongLevel2] [tinyint] NOT NULL,
[ActionType] [tinyint] NOT NULL,
[RegDate] [datetime] NOT NULL
) ON [PRIMARY]


