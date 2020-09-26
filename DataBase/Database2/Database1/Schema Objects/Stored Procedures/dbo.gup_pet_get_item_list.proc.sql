CREATE PROCEDURE [dbo].[gup_pet_get_item_list]
AS
/*microcat
작성일 : 2007년 1월 4일
사용 : 펫 아이템인지 아닌지 체크
*/
SET NOCOUNT ON;

SELECT
        GoodsID
,       Kind
    FROM dbo.GoodsInfoList(NOLOCK)
        WHERE Kind BETWEEN 50 AND 53


