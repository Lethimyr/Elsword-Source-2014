CREATE TABLE [dbo].[ClientChecksum]
(
[UID] [int] NOT NULL IDENTITY(1, 1),
[ProtocolVer] [int] NOT NULL,
[Checksum] [binary] (4) NOT NULL,
[RegDate] [smalldatetime] NOT NULL
) ON [PRIMARY]


