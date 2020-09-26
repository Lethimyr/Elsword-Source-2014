CREATE PROCEDURE [dbo].[lup_update_arcade_rank_server]
	@iRank		tinyint
,	@iGameMode	int
,	@iStageCnt	tinyint
,	@iPlayTime	int 
,	@sdtRegDate	smalldatetime
,	@strUnit1	nvarchar(16)
,	@strUnit2	nvarchar(16)
,	@strUnit3	nvarchar(16)
,	@strUnit4	nvarchar(16)
,	@iUnitUID1	bigint
,	@iUnitUID2	bigint
,	@iUnitUID3	bigint
,	@iUnitUID4	bigint
,	@iOK		int = 0



AS
SET NOCOUNT ON;
SET XACT_ABORT ON;

BEGIN TRAN
	IF NOT EXISTS (SELECT * FROM dbo.Rank_Arcade_Server WITH (NOLOCK) WHERE GameMode = @iGameMode AND Rank = @iRank)
		BEGIN
			INSERT INTO dbo.Rank_Arcade_Server (Rank, GameMode, StageCnt, PlayTime, RegDate, Unit1, Unit2, Unit3, Unit4, UnitUID1, UnitUID2, UnitUID3, UnitUID4)
				SELECT @iRank, @iGameMode, @iStageCnt, @iPlayTime, @sdtRegDate, @strUnit1, @strUnit2, @strUnit3, @strUnit4, @iUnitUID1, @iUnitUID2, @iUnitUID3, @iUnitUID4
			IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
				BEGIN	SELECT @iOK = -1	GOTO FAIL_TRAN	END
			GOTO COMMIT_TRAN
		END

		UPDATE dbo.Rank_Arcade_Server WITH (UPDLOCK)
			SET StageCnt = @iStageCnt,	
				PlayTime = @iPlayTime,	
				RegDate	= @sdtRegDate,	
				Unit1 = @strUnit1,
				Unit2 = @strUnit2,
				Unit3 = @strUnit3,	
				Unit4 = @strUnit4,	
				UnitUID1 = @iUnitUID1,	
				UnitUID2 = @iUnitUID2,	
				UnitUID3 = @iUnitUID3,
				UnitUID4 = @iUnitUID4
			WHERE GameMode = @iGameMode AND Rank = @iRank

COMMIT_TRAN:
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


