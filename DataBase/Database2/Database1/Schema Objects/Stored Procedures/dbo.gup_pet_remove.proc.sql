CREATE PROCEDURE [dbo].[gup_pet_remove]
	@iPetUID_ [int],
	@iLoginUID_ [int],
	@iPetID_ [int],
	@iOK [int] = 0
AS
/*microcat
작성일 : 2006년 12월 28일
사용 : 펫 제거
-5(펫이 존재하지 않음)
-11이하(트랜잭션 에러)
*/
SET NOCOUNT ON;

EXEC @iOK = dbo.gup_pet_check @iPetUID_, @iLoginUID_, @iPetID_
IF @iOK <> 0
    BEGIN    GOTO END_PROC    END

IF NOT EXISTS(SELECT * FROM dbo.GPet(NOLOCK) WHERE PetUID = @iPetUID_ AND LoginUID = @iLoginUID_ AND PetID = @iPetID_)
    BEGIN    SELECT @iOK = -5    GOTO END_PROC    END

BEGIN TRAN
    INSERT INTO dbo.GPetLog
        (
            PetUID
,           LoginUID
,           PetID
,           PetName
,           Promotion
,           [Exp]
,           FullTime
,           RegDate
        )
        SELECT
                a.PetUID
,               a.LoginUID
,               a.PetID
,               a.PetName
,               a.Promotion
,               b.[Exp]
,               a.FullTime
,               a.RegDate
            FROM dbo.GPet as a(NOLOCK)
                join dbo.GPetExp as b with (nolock)
                    on a.PetUID = b.PetUID and a.Promotion = b.Promotion and a.PetUID = @iPetUID_ AND a.LoginUID = @iLoginUID_ AND a.PetID = @iPetID_
    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
        BEGIN    SELECT @iOK = -11    GOTO FAIL_TRAN    END

    DELETE FROM dbo.GPet
        WHERE PetUID = @iPetUID_ AND LoginUID = @iLoginUID_ AND PetID = @iPetID_
    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
        BEGIN    SELECT @iOK = -12    GOTO FAIL_TRAN    END
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT  @iOK


