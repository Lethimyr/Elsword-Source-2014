CREATE PROCEDURE [dbo].[up_add_gp]
	@strLogin_ [nvarchar](20),
	@iGP_ [int]
AS
SET NOCOUNT ON

DECLARE     @iGP              int,
            @strResultSQL     nvarchar(200)

-- buyer의 존재, buyer의 GP를 구함
IF NOT EXISTS(SELECT * FROM dbo.users(NOLOCK) WHERE login = @strLogin_)
BEGIN
    SELECT -1, 0
    RETURN
END

BEGIN TRAN
    -- Buyer의 GP를 변경
       UPDATE dbo.users WITH (UPDLOCK) SET GamePoint = dbo.zcGamePoint(GamePoint + @iGP_) WHERE login = @strLogin_
       IF @@ERROR <> 0
           GOTO lbTran_Abort

COMMIT TRAN
GOTO lbTran_Success

lbTran_Abort:
ROLLBACK TRAN
SELECT -9, @iGP
RETURN

lbTran_Success:
SET @strResultSQL = 'SELECT 0,' + ' ' + CAST(@iGP AS nvarchar(10))
EXEC( @strResultSQL )
--PRINT @strResultSQL
RETURN


