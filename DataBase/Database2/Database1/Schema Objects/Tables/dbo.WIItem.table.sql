CREATE TABLE [dbo].[WIItem]
(
[IssueUID] [int] NOT NULL,
[UID] [int] NOT NULL IDENTITY(1, 1),
[ItemID] [int] NOT NULL,
[Factor] [int] NOT NULL,
[ExhaustionType] [tinyint] NOT NULL,
[PropertyType] [tinyint] NOT NULL
) ON [PRIMARY]


