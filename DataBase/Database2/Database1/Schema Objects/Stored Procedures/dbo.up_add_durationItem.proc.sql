CREATE PROCEDURE [dbo].[up_add_durationItem]
	@strLogin_ [nvarchar](20)
AS
exec dbo.up_log_set_proc_count 'up_add_durationItem'

SET NOCOUNT ON

DECLARE    @iCharType       tinyint,
           @iGoodsID        int,
           @iOK             int,
           @strResultSQL    nvarchar(200)

-- buyer의 존재, buyer의 GP를 구함
IF NOT EXISTS(SELECT * FROM dbo.users(NOLOCK) WHERE login = @strLogin_)
BEGIN
    SELECT -1
    RETURN
END

SELECT TOP 1 @iCharType = chartype FROM dbo.Characters(NOLOCK) WHERE login = @strLogin_ ORDER BY [exp] DESC

BEGIN TRAN
       --GoodsID값에서 종속성 제거 못함.

       SET @iGoodsID = CASE @iCharType
                            WHEN 0 THEN 4590
                            WHEN 1 THEN 4600
                            WHEN 2 THEN 4610
                            ELSE -1
                        END

--       IF @iCharType = 0
--        BEGIN
--            SET @iGoodsID = 4590
--        END
--       ELSE IF @iCharType = 1
--        BEGIN
--            SET @iGoodsID = 4600
--        END
--       ELSE IF @iCharType = 2
--        BEGIN
--            SET @iGoodsID = 4610
--        END
--       ELSE
--        BEGIN
--            SET @iGoodsID = 4590 --임시로 넣어둔 것임.
--        END
      
       exec @iOK = dbo.up_insert_item_for_cash @strLogin_, @strLogin_, @iGoodsID
       IF @@ERROR <> 0
           GOTO lbTran_Abort

	PRINT @iOK

COMMIT TRAN
GOTO lbTran_Success

lbTran_Abort:
ROLLBACK TRAN
SELECT -9
RETURN

lbTran_Success:
SET @strResultSQL = 'SELECT 0' 
EXEC( @strResultSQL )
--PRINT @strResultSQL
RETURN


