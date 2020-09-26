CREATE PROCEDURE [dbo].[fhup_invite_info_update]
	@iLoginUID_ [int],
	@iType_ [tinyint],
	@iOK [int] = 0
AS
/*microcat
작성일 : 2007년 4월 19일
사용 : FHInviteInfo의 정보를 갱신
*/
SET NOCOUNT ON;

EXEC @iOK = dbo.fhup_invite_available
IF @iOK = -1
    BEGIN    GOTO END_PROC    END

if @iType_ not between 0 and 4
    begin    select @iOK = -2    goto end_proc    end

if @iType_ between 1 and 4
    begin
        if not exists(select * from dbo.FHInviteInfo with (nolock) where LoginUIDB = @iLoginUID_ and Type = @iType_ and Checked = 0)
            BEGIN    SELECT @iOK = -3    goto end_proc    END
    
        declare @iCount int
        select  @iCount = RCount
            from dbo.FHInviteUser with (nolock)
                where LoginUIDB = @iLoginUID_
        if @iCount is null
            begin    select @iOK = -4    goto end_proc    end

        if (@iType_ = 1 and @iCount < 1)
            or (@iType_ = 2 and @iCount < 5)
            or (@iType_ = 3 and @iCount < 10)
            or (@iType_ = 4 and @iCount < 20)
            begin    select @iOK = -5    goto end_proc    end
    end
BEGIN TRAN
    UPDATE dbo.FHInviteInfo WITH (ROWLOCK)
        SET Checked = case @iType_ when 0 then 0 else 1 end
            WHERE LoginUIDB = @iLoginUID_ AND Type = @iType_
    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
        BEGIN    SELECT @iOK = -11    GOTO FAIL_TRAN    END
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


