CREATE PROCEDURE [dbo].[up_insert_user_for_Agreement]
	@login_ [nvarchar](20)
AS
exec dbo.up_log_set_proc_count 'up_insert_user_for_Agreement'

SET NOCOUNT ON

IF EXISTS( SELECT * FROM dbo.Agreement(NOLOCK) WHERE Login = @login_ ) 
BEGIN
  SELECT -1
  RETURN
END
ELSE -- 유저가 없다. 유저를 Agreement테이블에 넣는다.
BEGIN
  INSERT INTO dbo.Agreement( Login ) VALUES( @login_ ) 

  IF @@ERROR <> 0    GOTO lbTran_Abort
 
  SELECT 0
  RETURN
END

lbTran_Abort:
SELECT -2
RETURN


