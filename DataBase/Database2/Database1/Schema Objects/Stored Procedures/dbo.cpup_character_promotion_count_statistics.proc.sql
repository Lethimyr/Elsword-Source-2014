CREATE PROCEDURE [dbo].[cpup_character_promotion_count_statistics]
AS
/*microcat
작성일 : 2007년 7월 2일
사용 : 캐릭터별 전직별 플레이 카운트 통계
*/
set nocount on;

declare @sdtDate smalldatetime
select  @sdtDate = convert(nvarchar(10), GetDate() - 1, 120)

begin tran
    insert into dbo.CPStatCount
            (
                RegDate
,               CharType
,               Promotion
,               [Count]
            )
        select  @sdtDate
,               CharType
,               Promotion
,               [Count]
            from dbo.CPDailyCount with (updlock)
                where [Count] > 0
    if @@error <> 0
        begin    goto fail_tran    end

    delete a
        from dbo.CPDailyCount as a with (updlock)
            where [Count] = 0
    if @@error <> 0
        begin    goto fail_tran    end

    update a set a.[Count] = 0
        from dbo.CPDailyCount as a with (updlock)
    if @@error <> 0
        begin    goto fail_tran    end
commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:


