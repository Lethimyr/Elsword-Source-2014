CREATE PROCEDURE [dbo].[gup_pointbag_month_get]
	@iLoginUID_ [int]
AS
/*microcat
작성일 : 2007년 4월 23일
사용 : 유저의 월별 아이템 지급내역을 얻는다.
*/
SET NOCOUNT ON;

select  RegDate
,       ItemID
    from dbo.GPointBagMonth with (nolock)
        where LoginUID = @iLoginUID_
            and ItemID = -1


