CREATE PROCEDURE [dbo].[up_schedule_roundup_lotto]
AS
SET NOCOUNT ON
SET XACT_ABORT ON

TRUNCATE TABLE dbo.EventLottoP
INSERT INTO dbo.EventLottoP(Login, LottoID, Lotto1, Lotto2, Lotto3, Lotto4, Lotto5, Lotto, RegDate)
    SELECT Login, LottoID, Lotto1, Lotto2, Lotto3, Lotto4, Lotto5, Lotto, RegDate FROM dbo.EventLottoN(NOLOCK)

DECLARE @iRound int
SELECT @iRound = MAX([Round]) FROM dbo.EventLottoR(NOLOCK)
INSERT INTO dbo.EventLottoB(Login, [Round], LottoID, Lotto, RegDate)
    SELECT Login, @iRound, LottoID, Lotto, RegDate FROM dbo.EventLottoN(NOLOCK)

TRUNCATE TABLE dbo.EventLottoN
DBCC CHECKIDENT('EventLottoN', RESEED, 0)
INSERT INTO dbo.EventLottoR([Round]) VALUES(@iRound + 1)

UPDATE dbo.EventLottoU SET [Count] = 0


