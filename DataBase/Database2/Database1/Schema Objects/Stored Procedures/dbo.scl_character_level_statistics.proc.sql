CREATE PROCEDURE [dbo].[scl_character_level_statistics]
	@iOK [int] = 0,
	@iError [int] = 0
AS
/*microcat
작성일 : 2007년 10월 22일
사용 : 캐릭터별 레벨별 카운트 통계
*/
set nocount on;

declare @sdtDate smalldatetime
select  @sdtDate = convert(nvarchar(8), GetDate() - 1, 112)

declare @bExist bit
select  @bExist = 0
if exists
    (
        select *
            from dbo.SCLCharacterLevelStat with (nolock)
                where RegDate = @sdtDate
    )
    begin    select @bExist = 1    end

begin tran
    if @bExist = 1
        begin
            delete a with (updlock)
                from dbo.SCLCharacterLevelStat as a with (updlock)
                    where RegDate = @sdtDate
            if @@error <> 0
                begin    select @iOK = -101, @iError = @@error    goto fail_tran    end
        end

    insert into dbo.SCLCharacterLevelStat with (updlock)
            (
                RegDate
,               CharType
,               Level
,               CLCount
            )
        select  @sdtDate
,               CharType
,               Level
,               count(*)
            from dbo.Characters with (nolock) --원칙적으로 updlock이나 다른 프로세스에 영향이 큼
                group by CharType, Level
                    option (maxdop 1)
    if @@error <> 0
        begin    select @iOK = -102, @iError = @@error    goto fail_tran    end
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
                'microcat : %s.dbo.%s ( OK : %d, Error : %d )'
,               16, 127
,               @strDB, @strProc
,               @iOK, @iError
            )
    end


