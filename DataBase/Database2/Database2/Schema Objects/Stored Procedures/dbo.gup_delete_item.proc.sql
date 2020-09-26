CREATE PROCEDURE [dbo].[gup_delete_item]
    @iItemUID   [bigint],
    @iOK        [int] = 0
AS

SET NOCOUNT ON;

-- 생성된지 1분 안에 삭제될 경우에 삭제 시각이 생성 시각과 같아지는 것을 방지한다.
DECLARE @sdtNow smalldatetime
SELECT  @sdtNow = DateAdd( mi, 1, GetDate() )

BEGIN TRAN
    UPDATE dbo.GItem WITH( ROWLOCK )
        SET DelDate = @sdtNow
        WHERE ItemUID = @iItemUID
    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
    BEGIN
        SELECT @iOK = -2
        GOTO FAIL_TRAN
    END
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


