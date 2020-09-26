CREATE PROCEDURE [dbo].[glb_life_bonus_select]
	@iLoginUID_ [int]
AS
/*microcat
작성일 : 2007년 12월 14일
사용 : 유저의 라이프 보너스 정보 출력
*/
set nocount on;

if exists
    (
        select *
            from dbo.Users with (nolock)
                where LoginUID = @iLoginUID_
    )
    and not exists
    (
        select *
            from dbo.GLBLifeBonus with (nolock)
                where LoginUID = @iLoginUID_
    )
    begin
        begin tran
            insert into dbo.GLBLifeBonus with (updlock)
                    (
                        LoginUID
,                       DefaultLife
,                       SpecialLife
,                       RegDate
                    )
                select  @iLoginUID_
,                       10
,                       0
,                       getdate()
        commit tran
    end

select  DefaultLife
,       SpecialLife
    from dbo.GLBLifeBonus with (nolock)
        where LoginUID = @iLoginUID_


