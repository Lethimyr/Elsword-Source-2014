CREATE PROCEDURE [dbo].[up_indigo_query_by_login]
	@strLogin_ [nvarchar](20)
AS
SET NOCOUNT ON

-- 데이터 존재 확인
IF NOT EXISTS( SELECT * FROM dbo.IndigoRank( NOLOCK ) WHERE Login = @strLogin_ )
BEGIN
    SELECT -1, 1000, 0, 0, 0, -1   -- Users에 존재하지 않는 계정
    RETURN
END

SELECT 0, RP, Win, Lose, Rank FROM dbo.IndigoRank( NOLOCK ) WHERE Login = @strLogin_
RETURN


