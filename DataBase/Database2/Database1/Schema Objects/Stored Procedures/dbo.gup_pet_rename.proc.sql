CREATE PROCEDURE [dbo].[gup_pet_rename]
	@iPetUID_ [int],
	@iLoginUID_ [int],
	@iPetID_ [int],
	@strName_ [nvarchar](20),
	@iOK [int] = 0
AS
/*microcat
작성일 : 2007년 1월 13일
사용 : 펫 정보 갱신
-5(펫이 존재하지 않음)
-11이하(트랜잭션 에러)
*/
SET NOCOUNT ON;

EXEC @iOK = dbo.gup_pet_check @iPetUID_, @iLoginUID_, @iPetID_
IF @iOK <> 0
    BEGIN    GOTO END_PROC    END

DECLARE @strName nvarchar(20)
SELECT  @strName = PetName
    FROM dbo.GPet(NOLOCK)
        WHERE PetUID = @iPetUID_ AND LoginUID = @iLoginUID_ AND PetID = @iPetID_
IF @strName IS NULL
    BEGIN    SELECT @iOK = -5    GOTO END_PROC    END

IF @strName = @strName_
    BEGIN    SELECT @iOK = -6    GOTO END_PROC    END

BEGIN TRAN
    UPDATE dbo.GPet WITH (ROWLOCK)
        SET PetName = @strName_
            WHERE PetUID = @iPetUID_ AND LoginUID = @iLoginUID_ AND PetID = @iPetID_
    IF @@ERROR <> 0
        BEGIN    SELECT @iOK = -11    GOTO FAIL_TRAN    END

    INSERT INTO dbo.GPetNameLog
        (
            PetUID
,           OldPetName
,           NewPetName
        )
        SELECT
                @iPetUID_
,               @strName
,               @strName_
    IF @@ERROR <> 0
        BEGIN    SELECT @iOK = -12    GOTO FAIL_TRAN    END
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK


