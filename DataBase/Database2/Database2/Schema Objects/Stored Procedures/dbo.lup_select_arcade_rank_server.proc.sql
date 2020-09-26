CREATE PROCEDURE [dbo].[lup_select_arcade_rank_server]


AS
SET NOCOUNT ON;
SET XACT_ABORT ON;


	SELECT Rank, GameMode, StageCnt, PlayTime, RegDate, Unit1, Unit2, Unit3, Unit4, UnitUID1, UnitUID2, UnitUID3, UnitUID4
		FROM dbo.Rank_Arcade_Server WITH (NOLOCK)
			ORDER BY RANK OPTION (MAXDOP 1)


