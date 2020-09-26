CREATE TABLE [dbo].[UALUserAuth]
(
[LoginUID] [int] NOT NULL,
[RegDate] [datetime] NOT NULL,
[AuthLevel1] [int] NULL,
[LimitDate1] [smalldatetime] NULL,
[Status1] [int] NULL,
[AuthLevel2] [int] NULL,
[LimitDate2] [smalldatetime] NULL,
[Status2] [int] NULL,
[IPn] [bigint] NOT NULL
) ON [PRIMARY]


