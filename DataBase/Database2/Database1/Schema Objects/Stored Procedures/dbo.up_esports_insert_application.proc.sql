CREATE PROCEDURE [dbo].[up_esports_insert_application]
	@strLogin_ [nvarchar](20),
	@strName_ [nvarchar](20),
	@iYear_ [int],
	@iMonth_ [int],
	@iDate_ [int],
	@strPhoneNumber_ [nvarchar](50),
	@strEmail_ [nvarchar](50)
AS
exec dbo.up_log_set_proc_count 'up_esports_insert_application'

SEt NOCOUNT ON

DECLARE             @strApplicationBeginDay     nvarchar(20),   -- 아직 언제 신청 시작인지 모름.
                    @strApplicationEndDay       nvarchar(20)    -- 신청 기간은 8월 17일 까지.

SET @strApplicationBeginDay = '2005-07-28'
SET @strApplicationEndDay   = '2005-08-18'

-- 신청 기간이 맞는지 확인한다.
IF getdate() < @strApplicationBeginDay
BEGIN
    SELECT -2, '아직 신청 기간이 아닙니다.'
    RETURN
END
IF getdate() >= @strApplicationEndDay
BEGIN
    SELECT -3, '신청 기간이 지났습니다.'
    RETURN
END

-- 12세 이상인지 확인한다.
IF @iYear_ > 1994
BEGIN
    SELECT -4, '죄송합니다. 12세 미만은 대회에 참여할 수 없습니다. 다음 기회에 만나요.'
    RETURN
END

-- Grand Chase 계정이 있는지 확인한다.
IF NOT EXISTS( SELECT * FROM dbo.users( NOLOCK ) WHERE login = @strLogin_ )
BEGIN
    SELECT -5, '그랜드체이스 게임 계정이 없습니다. 게임을 플레이한 후에 신청해 주세요.'
    RETURN
END

-- 이미 신청하였는지 확인한다.
IF EXISTS( SELECT * FROM dbo.esports_PlayerList( NOLOCK ) WHERE Login = @strLogin_ )
BEGIN
    SELECT -1, '이미 참가 신청이 되어있습니다.'
    RETURN
END

BEGIN TRAN
    INSERT INTO dbo.esports_PlayerList( Login, Name, Year, Month, Day, Phone, EMail )
    VALUES( @strLogin_, @strName_, @iYear_, @iMonth_, @iDate_, @strPhoneNumber_, @strEmail_ )
    IF @@ERROR <> 0
        GOTO lbTran_Abort
COMMIT TRAN
GOTO lbTran_Success

lbTran_Abort:
ROLLBACK TRAN
SELECT -6, '신청을 할 수 없습니다.'
RETURN

lbTran_Success:
SELECT 0, '감사합니다. 대회 참가 신청이 정상적으로 완료되었습니다.'

RETURN


