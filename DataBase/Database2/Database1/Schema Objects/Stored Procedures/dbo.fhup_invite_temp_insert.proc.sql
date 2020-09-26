CREATE PROCEDURE [dbo].[fhup_invite_temp_insert]
	@strLogin_ [nvarchar](20),
	@bType_ [bit] = 0,
	@iOK [int] = 0
AS
/*microcat
작성일 : 2007년 4월 19일
사용 : 신규(0) 및 휴면(1) 등록
*/
SET NOCOUNT ON;

DECLARE @iLoginUID int
,       @sdtLastLogin smalldatetime
SELECT  @iLoginUID = LoginUID
,       @sdtLastLogin = LastLogin
    FROM dbo.Users WITH (NOLOCK)
        WHERE Login = @strLogin_

EXEC @iOK = dbo.fhup_invite_available
IF @iOK = -1
    BEGIN    RETURN    END

IF @bType_ = 1 AND @sdtLastLogin >= '070203'
    BEGIN    SELECT @iOK = -2    RETURN    END

if exists(select * from dbo.FHInviteTemp with (nolock) where LoginUIDA = @iLoginUID)
    begin    select @iOK = -3    return    end

BEGIN TRAN
    INSERT INTO dbo.FHInviteTemp
            (
                LoginUIDA
,               Destination
            )
        SELECT  @iLoginUID
,               @bType_
    if @@error <> 0 or @@rowcount <> 1
        begin    select @iOK = -11    goto fail_tran    end
COMMIT TRAN

goto end_proc

fail_tran:
rollback tran

end_proc:
return  @iOK


