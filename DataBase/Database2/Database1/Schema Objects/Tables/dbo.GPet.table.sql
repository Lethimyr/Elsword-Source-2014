CREATE TABLE [dbo].[GPet]
(
[PetUID] [int] NOT NULL,
[LoginUID] [int] NOT NULL,
[PetID] [int] NOT NULL,
[PetName] [nvarchar] (20) NOT NULL,
[Promotion] [tinyint] NOT NULL,
[FullTime] [int] NOT NULL,
[RegDate] [smalldatetime] NOT NULL
) ON [PRIMARY]


