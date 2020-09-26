CREATE PROCEDURE [dbo].[gup_update_arcade_rank]
	@iUnitUID	bigint
,	@iGameMode	int
,	@iStageCnt	tinyint
,	@iPlayTime	int 
,	@sdtRegDate	smalldatetime
,	@strUnit1	nvarchar(16)
,	@strUnit2	nvarchar(16)
,	@strUnit3	nvarchar(16)
,	@iOK		int = 0



AS
SET NOCOUNT ON;
SET XACT_ABORT ON;

BEGIN TRAN
	IF NOT EXISTS (SELECT TOP 1 UnitUID FROM dbo.GRank_Arcade WITH (NOLOCK) WHERE UnitUID = @iUnitUID AND GameMode = @iGameMode)
		BEGIN
			INSERT INTO dbo.GRank_Arcade (UnitUID, GameMode, StageCnt, PlayTime, RegDate, Unit1, Unit2, Unit3)
				SELECT @iUnitUID, @iGameMode, @iStageCnt, @iPlayTime, @sdtRegDate, @strUnit1, @strUnit2, @strUnit3



			IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
				BEGIN	SELECT @iOK = -1	GOTO FAIL_TRAN	END
			GOTO COMMIT_TRAN
		END

		UPDATE dbo.GRank_Arcade WITH (UPDLOCK)
			SET StageCnt = @iStageCnt,
				PlayTime = @iPlayTime,
				RegDate = @sdtRegDate,
				Unit1 = @strUnit1,
				Unit2 = @strUnit2,
				Unit3 = @strUnit3
				WHERE UnitUID = @iUnitUID AND GameMode = @iGameMode

	IF @@ERROR <> 0 OR @@ROWCOUNT <> 1 
		BEGIN	SELECT @iOK = -2	GOTO FAIL_TRAN	END


COMMIT_TRAN:
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


--CREATE TABLE dbo.GRank_Arcade
--(
--	UnitUID		bigint
--,	GameMode	int
--,	StageCnt	tinyint
--,	PlayTime	int
--,	RegDate		smalldatetime
--,	Unit1		nvarchar(16) DEFAULT NULL
--,	Unit2		nvarchar(16) DEFAULT NULL
--,	Unit3		nvarchar(16) DEFAULT NULL
--,	CONSTRAINT FK_GRank_Arcade_GUnit_UnitUID FOREIGN KEY (UnitUID) REFERENCES dbo.GUnit (UnitUID) ON UPDATE CASCADE ON DELETE CASCADE
--)
--
--create clustered index PKC_GRank_Arcade_UnitUID on dbo.GRank_Arcade (UnitUID) WITH FILLFACTOR = 80
--
--
--CREATE TABLE dbo.GRank_Arcade_Server
--(
--	Rank		tinyint
--,	GameMode	int
--,	StageCnt	tinyint
--,	PlayTime	int
--,	RegDate		smalldatetime
--,	Unit1		nvarchar(16)
--,	Unit2		nvarchar(16) DEFAULT NULL
--,	Unit3		nvarchar(16) DEFAULT NULL
--,	Unit4		nvarchar(16) DEFAULT NULL
--,	UnitUID1	bigint
--,	UnitUID2	bigint
--,	UnitUID3	bigint
--,	UnitUID4	bigint
--)
--


