CREATE PROCEDURE [dbo].[lup_delete_arcade_rank_server]

AS
SET NOCOUNT ON;
SET XACT_ABORT ON;

DELETE FROM dbo.GRank_Arcade_Server
	WHERE RANK BETWEEN 1 AND 5


