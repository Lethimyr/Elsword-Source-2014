CREATE PROCEDURE [dbo].[gup_update_rank]
	@iOK_	[int] = 0
/* 
sutamtwo
작성일 : 2008년 2월 13일
사용 : GameDB용 랭킹 갱신에 사용될 SP
*/

AS
SET NOCOUNT ON;
SET XACT_ABORT ON;

BEGIN TRAN
	-- 전체 랭킹 갱신
	TRUNCATE TABLE dbo.GRank_Old
		IF @@ERROR <> 0
		BEGIN	SELECT @iOK_ = -101	GOTO FAIL_TRAN	END
	INSERT INTO dbo.GRank_Old (unitUID, win, lose, [Level], [exp], ExpRank, Nickname, unitclass)
		SELECT a.unituid, a.win, a.lose, a.level, a.[exp],  rank() over(order by [exp] desc) , b.nickname, a.unitclass
			FROM dbo.gunit as a with (nolock)
				JOIN dbo.gunitnickname as b with (nolock)
				on a.deleted = 0 and a.unituid = b.unituid
					and a.unituid not in (2469,16225)
					ORDER BY 2 desc ,3 asc, 4 desc, 1 asc
--	UPDATE dbo.GRank_Old
--		SET UnitUID = null


		IF @@ERROR <> 0
		BEGIN	SELECT @iOK_ = -102	GOTO FAIL_TRAN	END

	EXEC sp_rename 'GRank_', 'GRank_Temp'
	EXEC sp_rename 'GRank_Old', 'GRank_'
	EXEC sp_rename 'GRank_Temp', 'GRank_Old'

	-- 엘소드 랭킹 갱신
	TRUNCATE TABLE dbo.GRank_Elsword_Old
		IF @@ERROR <> 0
		BEGIN	SELECT @iOK_ = -201	GOTO FAIL_TRAN	END		

	INSERT INTO dbo.GRank_Elsword_Old (unitUID, win, lose, [Level], [exp], ExpRank, Nickname)
		SELECT a.unituid, a.win, a.lose, a.level, a.[exp],  rank() over(order by [exp] desc) , b.nickname
			FROM dbo.gunit as a with (nolock)
				JOIN dbo.gunitnickname as b with (nolock)
				ON a.unitclass = 1 and a.deleted = 0 and a.unituid = b.unituid
					and a.unituid not in (2469,16225)
					ORDER BY 2 desc ,3 asc, 4 desc, 1 asc	
		IF @@ERROR <> 0
		BEGIN	SELECT @iOK_ = -211	GOTO FAIL_TRAN	END						

--	UPDATE dbo.GRank_Elsword_Old
--		SET UnitUID = null

	EXEC sp_rename 'GRank_Elsword', 'GRank_Elsword_Temp'
	EXEC sp_rename 'GRank_Elsword_Old', 'GRank_Elsword'
	EXEC sp_rename 'GRank_Elsword_Temp', 'GRank_Elsword_Old'

	-- 레나 랭킹 갱신
	TRUNCATE TABLE dbo.GRank_Rena_Old
		IF @@ERROR <> 0
		BEGIN	SELECT @iOK_ = -202	GOTO FAIL_TRAN	END
		
	INSERT INTO dbo.GRank_Rena_Old (unitUID, win, lose, [Level], [exp], ExpRank, Nickname)
		SELECT a.unituid, a.win, a.lose, a.level, a.[exp],  rank() over(order by [exp] desc) , b.nickname
			FROM dbo.gunit as a with (nolock)
				JOIN dbo.gunitnickname as b with (nolock)
				ON a.unitclass = 3 and a.deleted = 0 and a.unituid = b.unituid
					and a.unituid not in (2469,16225)
					ORDER BY 2 desc ,3 asc, 4 desc, 1 asc					
		IF @@ERROR <> 0
		BEGIN	SELECT @iOK_ = -212	GOTO FAIL_TRAN	END						

--	UPDATE dbo.GRank_Rena_Old
--		SET UnitUID = null

	EXEC sp_rename 'GRank_Rena', 'GRank_Rena_Temp'
	EXEC sp_rename 'GRank_Rena_Old', 'GRank_Rena'
	EXEC sp_rename 'GRank_Rena_Temp', 'GRank_Rena_Old'

	-- 아이샤 랭킹 갱신
	TRUNCATE TABLE dbo.GRank_Aisha_Old
		IF @@ERROR <> 0
		BEGIN	SELECT @iOK_ = -203	GOTO FAIL_TRAN	END

	INSERT INTO dbo.GRank_Aisha_Old (unitUID, win, lose, [Level], [exp], ExpRank, Nickname)
		SELECT a.unituid, a.win, a.lose, a.level, a.[exp],  rank() over(order by [exp] desc) , b.nickname
			FROM game01.dbo.gunit as a with (nolock)
				JOIN game01.dbo.gunitnickname as b with (nolock)
				ON a.unitclass = 2 and a.deleted = 0 and a.unituid = b.unituid
					and a.unituid not in (2469,16225)
					ORDER BY 2 desc ,3 asc, 4 desc, 1 asc					

		IF @@ERROR <> 0
		BEGIN	SELECT @iOK_ = -213	GOTO FAIL_TRAN	END						

--	UPDATE dbo.GRank_Aisha_Old
--		SET UnitUID = null

	EXEC sp_rename 'GRank_Aisha', 'GRank_Aisha_Temp'
	EXEC sp_rename 'GRank_Aisha_Old', 'GRank_Aisha'
	EXEC sp_rename 'GRank_Aisha_Temp', 'GRank_Aisha_Old'	

COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK_


