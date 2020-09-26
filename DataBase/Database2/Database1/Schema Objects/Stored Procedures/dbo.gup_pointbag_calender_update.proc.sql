CREATE PROCEDURE [dbo].[gup_pointbag_calender_update]
	@iLoginUID_ [int],
	@strData_ [nvarchar](4000),
	@iOK [int] = 0
AS
/*microcat
작성일 : 2007년 4월 12일
사용 : 유저의 포인트백 달력 정보를 갱신한다.
*/
SET NOCOUNT ON;

DECLARE @strToken nvarchar(1)
SELECT  @strToken = N','

DECLARE @Calender TABLE
(
    RegDate smalldatetime not null primary key clustered
,   SealType tinyint not null
)

IF @strData_ <> N''
    BEGIN
        WHILE RIGHT(@strData_, 1) = @strToken
            BEGIN
                SELECT @strData_ = SubString(@strData_, 1, Len(@strData_) - 1)
            END

        DECLARE @sdtRegDate smalldatetime
,               @iSealType tinyint
,               @iStart int
,               @iEnd int
        SELECT
                @iStart = 1
,               @iEnd = 1
        WHILE 1 = 1
            BEGIN
                SELECT  @iEnd = CharIndex(@strToken, @strData_, @iStart + 1)
                IF @iEnd = 0
                    BEGIN    SELECT @iOK = -1    GOTO END_PROC    END
                SELECT  @sdtRegDate = SubString(@strData_, @iStart, @iEnd - @iStart)
                SELECT  @iStart = @iEnd + 1

                SELECT  @iEnd = CharIndex(@strToken, @strData_, @iStart + 1)
                IF @iEnd = 0
                    BEGIN    SELECT @iEnd = 4000    END
                SELECT  @iSealType = SubString(@strData_, @iStart, @iEnd - @iStart)

                INSERT INTO @Calender
                        (
                            RegDate
,                           SealType
                        )
                    SELECT
                            @sdtRegDate
,                           @iSealType

                IF @iEnd = 4000
                    BEGIN    BREAK    END

                SELECT @iStart = @iEnd + 1
            END
    END

IF NOT EXISTS(SELECT * FROM @Calender)
    BEGIN    SELECT @iOK = -2    GOTO END_PROC    END


BEGIN TRAN
    UPDATE a
        SET a.SealType = b.SealType
            FROM dbo.GPointBagCalender as a WITH (NOLOCK)
                JOIN @Calender as b
                    ON a.LoginUID = @iLoginUID_ AND a.RegDate = b.RegDate
    IF @@ERROR <> 0
        BEGIN    SELECT @iOK = -11    GOTO FAIL_TRAN    END

    INSERT INTO dbo.GPointBagCalender
            (
                LoginUID
,               RegDate
,               SealType
            )
        SELECT  @iLoginUID_
,               a.RegDate
,               a.SealType
            FROM @Calender as a
                WHERE NOT EXISTS(SELECT * FROM dbo.GPointBagCalender as b WITH (NOLOCK) WHERE b.LoginUID = @iLoginUID_ AND a.RegDate = b.RegDate)
    IF @@ERROR <> 0
        BEGIN    SELECT @iOK = -12    GOTO FAIL_TRAN    END
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


