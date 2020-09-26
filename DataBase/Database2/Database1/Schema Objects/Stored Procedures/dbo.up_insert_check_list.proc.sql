CREATE PROCEDURE [dbo].[up_insert_check_list]
	@iLoginUID_ [int],
	@vbList_ [varbinary](48),
	@bModify_ [bit] = 0,
	@iOK [int] = 0
AS
SET NOCOUNT ON;

IF NOT EXISTS(SELECT * FROM dbo.Users(NOLOCK) WHERE LoginUID = @iLoginUID_)
    BEGIN    SELECT @iOK = -1    GOTO END_PROC    END

IF @bModify_ = 0 AND EXISTS(SELECT * FROM dbo.CheckList(NOLOCK) WHERE LoginUID = @iLoginUID_)
    BEGIN    SELECT @iOK = -2    GOTO END_PROC    END

IF @bModify_ = 1 AND NOT EXISTS(SELECT * FROM dbo.CheckList(NOLOCK) WHERE LoginUID = @iLoginUID_)
    BEGIN    SELECT @iOK = -3    GOTO END_PROC    END

BEGIN TRAN
    IF @bModify_ = 0
        BEGIN
            INSERT INTO dbo.CheckList(LoginUID, List)
                SELECT @iLoginUID_, @vbList_
            IF @@ERROR <> 0
                BEGIN    SELECT @iOK = -11    GOTO FAIL_TRAN    END
        END
    ELSE IF @bModify_ = 1
        BEGIN
            UPDATE dbo.CheckList WITH (ROWLOCK)
                SET List = @vbList_
                    WHERE LoginUID = @iLoginUID_
            IF @@ERROR <> 0
                BEGIN    SELECT @iOK = -12    GOTO FAIL_TRAN    END
        END
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


