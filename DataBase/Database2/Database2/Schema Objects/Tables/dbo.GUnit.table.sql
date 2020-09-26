CREATE TABLE [dbo].[GUnit]
(
[UnitUID] [bigint] NOT NULL IDENTITY(1, 1),
[UserUID] [bigint] NOT NULL,
[UnitClass] [tinyint] NOT NULL,
[Exp] [int] NOT NULL,
[Level] [tinyint] NOT NULL,
[GamePoint] [int] NOT NULL,
[VSPoint] [int] NOT NULL,
[VSPointMax] [int] NULL,
[BaseHP] [smallint] NOT NULL,
[AtkPhysic] [tinyint] NOT NULL,
[AtkMagic] [tinyint] NOT NULL,
[DefPhysic] [tinyint] NOT NULL,
[DefMagic] [tinyint] NOT NULL,
[SPoint] [int] NOT NULL,
[Win] [int] NOT NULL,
[Lose] [int] NOT NULL,
[Seceder] [int] NOT NULL,
[RegDate] [smalldatetime] NOT NULL,
[DelDate] [smalldatetime] NOT NULL,
[LastDate] [smalldatetime] NOT NULL,
[LastPosition] [int] NOT NULL,
[PlayDayCnt] [int] NOT NULL,
[LoginCount] [int] NOT NULL,
[Deleted] AS (case [RegDate] when [DelDate] then (0) else (1) end)
) ON [PRIMARY]


