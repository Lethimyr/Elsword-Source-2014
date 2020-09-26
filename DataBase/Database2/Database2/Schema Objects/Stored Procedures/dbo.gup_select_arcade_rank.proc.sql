CREATE PROCEDURE [dbo].[gup_select_arcade_rank]
	@iUnitUID	bigint

AS
SET NOCOUNT ON;
SET XACT_ABORT ON;

SELECT GameMode, StageCnt, PlayTime, RegDate, Unit1, Unit2, Unit3
	FROM dbo.GRank_Arcade WITH (NOLOCK)
		WHERE UnitUID = @iUnitUID
			OPTION (MAXDOP 1)


