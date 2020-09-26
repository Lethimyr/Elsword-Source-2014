CREATE PROCEDURE [dbo].[gup_pointbag_attendance_update]
	@iLoginUID_ [int],
	@iPlayTime_ [int],
	@sdtRegDate_ [smalldatetime],
	@iOK [int] = 0
AS
/*microcat
작성일 : 2007년 4월 12일
사용 : 유저의 포인트백 정보를 갱신한다.
*/
SET NOCOUNT ON;

if not exists(select * from dbo.GPointBag with (nolock) where LoginUID = @iLoginUID_)
    begin    select @iOK = -1    goto end_proc    end

BEGIN TRAN
    UPDATE dbo.GPointBagAttendance WITH (ROWLOCK)
        SET PlayTime = @iPlayTime_
,           RegDate = @sdtRegDate_
            WHERE LoginUID = @iLoginUID_
    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
        BEGIN    SELECT @iOK = -11    GOTO FAIL_TRAN    END
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


