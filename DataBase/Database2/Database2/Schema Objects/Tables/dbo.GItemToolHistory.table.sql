CREATE TABLE [dbo].[GItemToolHistory]
(
[UnitUID] [bigint] NOT NULL,
[Itemuid] [bigint] NOT NULL,
[InsOrDel] [bit] NOT NULL,
[session] [nvarchar] (20) NOT NULL,
[IP] [nvarchar] (15) NOT NULL,
[regdate] [smalldatetime] NOT NULL
) ON [PRIMARY]


