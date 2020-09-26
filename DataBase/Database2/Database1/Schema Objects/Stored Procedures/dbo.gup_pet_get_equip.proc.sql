CREATE PROCEDURE [dbo].[gup_pet_get_equip]
	@iLoginUID_ [int]
AS
/*microcat
작성일 : 2006년 12월 28일
사용 : 펫의 장착 정보 출력
*/
SET NOCOUNT ON;

SELECT
        a.PetUID
,       a.ItemUID
,       a.ItemType
    FROM dbo.GPetEquip as a(NOLOCK)
        WHERE EXISTS(SELECT * FROM dbo.GPet as b(NOLOCK) WHERE a.PetUID = b.PetUID AND b.LoginUID = @iLoginUID_)


