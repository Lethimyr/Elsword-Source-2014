CREATE PROCEDURE [dbo].[gup_pet_check]
	@iPetUID_ [int],
	@iLoginUID_ [int],
	@iPetID_ [int],
	@iOK [int] = 0
AS
/*microcat
작성일 : 2006년 12월 28일
사용 : 펫, 유저가 존재하는지 펫 아이템이 존재하는지... 체크
 0(성공)
-1(유저가 존재하지 않음)
-2(이 아이템이 펫 아이템일까요?)
-3(펫 아이템이 존재하지 않음)
*/
SET NOCOUNT ON;

DECLARE @strLogin nvarchar(20)
SELECT  @strLogin = Login
    FROM dbo.Users(NOLOCK)
        WHERE LoginUID = @iLoginUID_

IF @strLogin IS NULL
    BEGIN    SELECT @iOK = -1    GOTO END_PROC    END

IF NOT EXISTS(SELECT * FROM dbo.GoodsInfoList(NOLOCK) WHERE GoodsID = @iPetID_ AND Kind = 50)
    BEGIN    SELECT @iOK = -2    GOTO END_PROC    END

IF NOT EXISTS(SELECT * FROM dbo.GoodsObjectList(NOLOCK) WHERE ItemUID = @iPetUID_ AND OwnerLogin = @strLogin AND ItemID = @iPetID_)
    BEGIN    SELECT @iOK = -3    GOTO END_PROC    END

END_PROC:
RETURN  @iOK


