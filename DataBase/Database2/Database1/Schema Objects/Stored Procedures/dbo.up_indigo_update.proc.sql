CREATE PROCEDURE [dbo].[up_indigo_update]
	@iLoginUID_ [int],
	@iRPChange_ [int],
	@iWinChange_ [int],
	@iLoseChange_ [int]
AS
SET NOCOUNT ON

-- 데이터 업데이트
UPDATE dbo.IndigoUser WITH(UPDLOCK) SET 
    RP      = RP + @iRPChange_,
    Win     = Win + @iWinChange_,
    Lose    = Lose + @iLoseChange_,
    WinRate = dbo.fnWinRate( Win + @iWinChange_, Lose + @iLoseChange_ )
    WHERE LoginUID = @iLoginUID_

IF @@ROWCOUNT = 0
BEGIN
    SELECT -1   -- IndigoUser에 존재하지 않는 계정
    RETURN
END
    
SELECT 0, RP, Win, Lose, Rank FROM dbo.IndigoUser( NOLOCK ) WHERE LoginUID = @iLoginUID_

RETURN


