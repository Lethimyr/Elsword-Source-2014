CREATE TABLE [dbo].[DescModeCount]
(
[ModeCode] [int] NOT NULL,
[ModeType] AS (([ModeCode] & 0xffff0000) / 65536),
[Difficulty] AS ([ModeCode] & 0x0000ffff),
[ModeName] [nvarchar] (20) NOT NULL
) ON [PRIMARY]


