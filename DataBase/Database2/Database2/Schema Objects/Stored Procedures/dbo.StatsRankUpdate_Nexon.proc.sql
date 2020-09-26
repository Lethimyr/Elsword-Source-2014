CREATE PROCEDURE [dbo].[StatsRankUpdate_Nexon]
	@iOK_	[int] = 0
/* 
sutamtwo
작성일 : 2008년 02월 13일
사용 : Nexon DB용 랭킹 갱신에 사용될 SP
*/

AS
SET NOCOUNT ON;
SET XACT_ABORT ON;

BEGIN TRAN

	-- 전체 랭킹 갱신
	TRUNCATE TABLE [Nexon].dbo.Rank_
	INSERT INTO [Nexon].dbo.Rank_ (Nickname, VsRank, ExpRank, win, lose, [exp], unitclass)
		SELECT Nickname, VsRank, ExpRank, win, lose, [exp], unitclass
			FROM game01.dbo.GRank_ as a with (nolock)

		IF @@ERROR <> 0
		BEGIN	SELECT @iOK_ = -101	GOTO FAIL_TRAN	END


	-- 엘소드 랭킹 갱신
	TRUNCATE TABLE [Nexon].dbo.Rank_Elsword
	INSERT INTO [Nexon].dbo.Rank_Elsword (Nickname, VsRank, ExpRank, win, lose, [exp])
		SELECT Nickname, VsRank, ExpRank, win, lose, [exp]
			FROM game01.dbo.GRank_Elsword as a with (nolock)

		IF @@ERROR <> 0
		BEGIN	SELECT @iOK_ = -102	GOTO FAIL_TRAN	END


	-- 아이샤 랭킹 갱신
	TRUNCATE TABLE [Nexon].dbo.Rank_Aisha
	INSERT INTO [Nexon].dbo.Rank_Aisha (Nickname, VsRank, ExpRank, win, lose, [exp])
		SELECT Nickname, VsRank, ExpRank, win, lose, [exp]
			FROM game01.dbo.GRank_Aisha as a with (nolock)

		IF @@ERROR <> 0
		BEGIN	SELECT @iOK_ = -103	GOTO FAIL_TRAN	END

	-- 레나 랭킹 갱신
	TRUNCATE TABLE [Nexon].dbo.Rank_Rena
	INSERT INTO [Nexon].dbo.Rank_Rena (Nickname, VsRank, ExpRank, win, lose, [exp])
		SELECT Nickname, VsRank, ExpRank, win, lose, [exp]
			FROM game01.dbo.GRank_Rena as a with (nolock)

		IF @@ERROR <> 0
		BEGIN	SELECT @iOK_ = -104	GOTO FAIL_TRAN	END


COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK_


