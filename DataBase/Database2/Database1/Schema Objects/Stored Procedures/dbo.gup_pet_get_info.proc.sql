CREATE PROCEDURE [dbo].[gup_pet_get_info]
	@iLoginUID_ [int]
AS
/*microcat
작성일 : 2006년 12월 28일
사용 : 펫 리스트 출력
*/
SET NOCOUNT ON;

SELECT
        PetUID
,       PetID
,       PetName
,       Promotion
,       FullTime
    FROM dbo.GPet(NOLOCK)
        WHERE LoginUID = @iLoginUID_


