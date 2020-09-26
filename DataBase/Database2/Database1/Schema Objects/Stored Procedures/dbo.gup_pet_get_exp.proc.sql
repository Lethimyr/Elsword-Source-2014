CREATE PROCEDURE [dbo].[gup_pet_get_exp]
	@iLoginUID_ [int]
AS
/*microcat
작성일 : 2007년 4월 19일
사용 : 펫 경험치 리스트 출력
*/
SET NOCOUNT ON;

SELECT  a.PetUID
,       a.Promotion
,       a.[Exp]
    FROM dbo.GPetExp as a WITH (NOLOCK)
        WHERE EXISTS(SELECT * FROM dbo.GPet as b WITH (NOLOCK) WHERE a.PetUID = b.PetUID AND b.LoginUID = @iLoginUID_)


