CREATE PROCEDURE [dbo].[up_change_DurationItem]
	@strOwnerLogin_ [nvarchar](20),
	@iGoodsID_ [int],
	@iDurationItemUID_ [int],
	@iDurationNum_ [int]
AS
exec dbo.up_log_set_proc_count 'up_change_DurationItem'

SET NOCOUNT ON

--DECLARE @strResultSQL nvarchar(12)

IF @iDurationNum_ <= 0 
BEGIN
   SELECT -3
   RETURN 
END

BEGIN TRAN
  UPDATE dbo.DurationItemObjectList WITH(ROWLOCK)
  SET Duration = @iDurationNum_
  WHERE OwnerLogin = @strOwnerLogin_ AND DurationItemUID = @iDurationItemUID_
  IF @@ERROR <> 0
      GOTO lbTran_Abort
COMMIT TRAN
GOTO lbTran_Success

lbTran_Abort:
ROLLBACK TRAN
SELECT -5
RETURN 

lbTran_Success:
SELECT 0
--SET @strResultSQL = 'SELECT 0,' + ' ' + @iDurationNum_
--EXEC( @strResultSQL )
--PRINT @strResultSQL
RETURN


