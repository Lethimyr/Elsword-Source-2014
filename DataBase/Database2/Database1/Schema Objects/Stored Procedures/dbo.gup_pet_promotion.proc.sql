CREATE PROCEDURE [dbo].[gup_pet_promotion]
	@iPetUID_ [int],
	@iLoginUID_ [int],
	@iPetID_ [int],
	@strPetName_ [nvarchar](20),
	@iPromotion_ [tinyint],
	@iOK [int] = 0
AS
/*microcat
작성일 : 2007년 4월 6일
사용 : 펫 전직 또는 퇴화
-5(펫이 생성되지 않았음)
-6(펫 전직레벨이 이상함)
-11이하(트랜잭션 에러)
*/
SET NOCOUNT ON;

EXEC @iOK = dbo.gup_pet_check @iPetUID_, @iLoginUID_, @iPetID_
IF @iOK <> 0
    BEGIN    GOTO END_PROC    END

DECLARE @strPetName nvarchar(20)
,       @iPromotion tinyint
SELECT  @strPetName = PetName
,       @iPromotion = Promotion
    FROM dbo.GPet WITH (NOLOCK)
        WHERE PetUID = @iPetUID_ AND LoginUID = @iLoginUID_ AND PetID = @iPetID_
IF @strPetName is Null
    BEGIN    SELECT @iOK = -5    GOTO END_PROC    END

IF NOT EXISTS(SELECT * FROM dbo.GPetPromotion WITH (NOLOCK) WHERE Promotion = @iPromotion_)
    BEGIN    SELECT @iOK = -6    GOTO END_PROC    END

BEGIN TRAN
    UPDATE dbo.GPet WITH (ROWLOCK)
        SET PetName = @strPetName_
,           Promotion = @iPromotion_
            WHERE PetUID = @iPetUID_ AND LoginUID = @iLoginUID_ AND PetID = @iPetID_
    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
        BEGIN    SELECT @iOK = -11    GOTO FAIL_TRAN    END

    INSERT INTO dbo.GPetPromotionLog
            (
                PetUID
,               LoginUID
,               PetID
,               OldPetName
,               NewPetName
,               OldPromotion
,               NewPromotion
            )
        SELECT  @iPetUID_
,               @iLoginUID_
,               @iPetID_
,               @strPetName
,               @strPetName_
,               @iPromotion
,               @iPromotion_
    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1
        BEGIN    SELECT @iOK = -12    GOTO FAIL_TRAN    END


COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT  @iOK


