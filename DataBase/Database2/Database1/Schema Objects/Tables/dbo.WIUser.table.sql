CREATE TABLE [dbo].[WIUser]
(
[LoginUID] [int] NOT NULL,
[UID] [int] NOT NULL IDENTITY(1, 1),
[SupplierUID] [int] NOT NULL,
[IssueUID] [int] NOT NULL,
[RegDate] [smalldatetime] NOT NULL,
[SupDate] [smalldatetime] NOT NULL,
[Supplied] AS (case when ([RegDate] = [SupDate]) then 0 else 1 end)
) ON [PRIMARY]


