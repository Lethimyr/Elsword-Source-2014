CREATE PROCEDURE [dbo].[glb_life_bonus_update]
	@iLoginUID_ [int],
	@iDefaultLifeDiff_ [int],
	@iSpecialLifeDiff_ [int],
	@iOK [int] = 0,
	@iError [int] = 0
AS
/*microcat
작성일 : 2007년 12월 14일
사용 : 유저의 라이프 보너스 정보 갱신
*/
set nocount on;

if not exists
    (
        select *
            from dbo.Users with (nolock)
                where LoginUID = @iLoginUID_
    )
    begin    select @iOK = -1    goto end_proc    end

if not exists
    (
        select *
            from dbo.GLBLifeBonus with (nolock)
                where LoginUID = @iLoginUID_
    )
    begin    select @iOK = -2    goto end_proc    end


begin tran
    update a with (updlock)
        set DefaultLife = case when DefaultLife + @iDefaultLifeDiff_ > 99 then 99 when DefaultLife + @iDefaultLifeDiff_ < 0 then 0 else DefaultLife + @iDefaultLifeDiff_ end
,           SpecialLife = case when SpecialLife + @iSpecialLifeDiff_ > 99 then 99 when SpecialLife + @iSpecialLifeDiff_ < 0 then 0 else SpecialLife + @iSpecialLifeDiff_ end
,           RegDate = getdate()
            from dbo.GLBLifeBonus as a with (updlock)
                where LoginUID = @iLoginUID_
    if @@error <> 0
        begin    select @iOK = -101, @iError = @@error    goto fail_tran    end
commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:
if @iOK <> 0
    begin
        declare @strDB nvarchar(4000)
,               @strProc nvarchar(4000)
        select  @strDB = db_name()
,               @strProc = object_name(@@procid)
        raiserror
            (
                'microcat : %s.dbo.%s( OK : %d, Error : %d / LoginUID : %d, DefaultLife : %d, SpecialLife : %d )'
,               16, 127, @strDB, @strProc, @iOK, @iError
,               @iLoginUID_
,               @iDefaultLifeDiff_
,               @iSpecialLifeDiff_
            )
    end
select @iOK


