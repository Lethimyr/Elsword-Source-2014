CREATE PROCEDURE [dbo].[up_update_user_mode_count]
	@strData_ [nvarchar](4000),
	@iOK [int] = 0
AS
/*microcat
작성일 : 2006년 12월 11일
사용 : 게임서버
*/
SET NOCOUNT ON;

DECLARE @UserModeCount TABLE
(
    ModeCode int not null primary key
,   ModeCount int not null
)

WHILE RIGHT(@strData_, 1) = ','
    BEGIN
        SELECT @strData_ = SubString(@strData_, 1, Len(@strData_) - 1)
    END

DECLARE @strToken nvarchar(1)
,       @iStart int
,       @iMid int
,       @iEnd int
SELECT  @strToken = N','
,       @iStart = 1
,       @iMid = 1
,       @iEnd = 1
WHILE 1 = 1
    BEGIN
        SELECT  @iMid = CharIndex(@strToken, @strData_, @iStart)
,               @iEnd = CharIndex(@strToken, @strData_, @iMid + 1)

        IF @iMid = 0
            BEGIN    SELECT @iOK = -1    GOTO END_PROC    END
        IF @iEnd = 0
            BEGIN    SELECT @iEnd = 4000    END

        INSERT INTO @UserModeCount(ModeCode, ModeCount) --모드가 중복해서 들어오지 않는다고 가정함 -__;;;
            SELECT  SubString(@strData_, @iStart, @iMid - @iStart)
,                   SubString(@strData_, @iMid + 1, @iEnd - @iMid - 1)

        IF @iEnd = 4000
            BEGIN    BREAK    END

        SELECT @iStart = @iEnd + 1
    END

BEGIN TRAN
    UPDATE b with (updlock)
        SET b.ModeCount = a.ModeCount + b.ModeCount
            FROM @UserModeCount as a
                JOIN dbo.UserModeCount as b with (updlock)
                    ON a.ModeCode = b.ModeCode
    IF @@ERROR <> 0
        BEGIN    SELECT @iOK = -11    GOTO FAIL_TRAN    END

    INSERT INTO dbo.UserModeCount with (updlock)
            (
                ModeCode
,               ModeCount
            ) 
        SELECT  a.ModeCode
,               a.ModeCount
            FROM @UserModeCount as a
                LEFT OUTER JOIN dbo.UserModeCount as b with (updlock)
                    ON a.ModeCode = b.ModeCode
                        WHERE b.ModeCode IS NULL
    IF @@ERROR <> 0
        BEGIN    SELECT @iOK = -12    GOTO FAIL_TRAN    END
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


