CREATE PROCEDURE [dbo].[up_indigo_query_by_uid]
	@iLoginUID_ [int]
AS
SET NOCOUNT ON

-- 데이터 존재 확인
IF NOT EXISTS( SELECT * FROM dbo.IndigoUser( NOLOCK ) WHERE LoginUID = @iLoginUID_ )
BEGIN
    SELECT -1, 1000, 0, 0, -1   -- Users에 존재하지 않는 계정
    RETURN
END

SELECT 0, RP, Win, Lose, Rank FROM dbo.IndigoUser( NOLOCK ) WHERE LoginUID = @iLoginUID_
RETURN


