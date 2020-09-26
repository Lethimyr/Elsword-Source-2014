CREATE TABLE [dbo].[StatModeCount]
(
[RegDate] [smalldatetime] NOT NULL,
[ModeCode] [int] NOT NULL,
[ModeType] AS (([ModeCode] & 0xffff0000) / 65536),
[Difficulty] AS ([ModeCode] & 0x0000ffff),
[ModeCount] [int] NOT NULL
) ON [PRIMARY]


