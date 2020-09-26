CREATE PROCEDURE [dbo].[gup_pointbag_luckyday_item_list_get]
	@sdtRegDate_ [smalldatetime]
AS
/*microcat
작성일 : 2007년 6월 14일
사용 : 포인트몰의 특정일에 대한 아이템 목록을 리턴
*/
SET NOCOUNT ON;

select  ItemID
,       ItemType
,       Factor
    from dbo.GPointBagLuckyDayItem with (nolock)
        where RegDate = @sdtRegDate_


