CREATE PROCEDURE [dbo].[RGSC_rank_grandchaser_schedule]
	@bWeb [bit] = 0,
	@bSupply [bit] = 1,
	@bDaily [bit] = 1,
	@bWeekly [bit] = 0,
	@bMonthly [bit] = 0,
	@bExp [bit] = 1,
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

declare
    @sdtDate smalldatetime

select
    @sdtDate = convert(nvarchar(10), current_timestamp, 120)


if @bDaily = 1
begin
    truncate table dbo.RGRRankGrandchaserDaily_old
    drop index dbo.RGRRankGrandchaserDaily_old.IX_RGRRankGrandchaser_Nick
    drop index dbo.RGRRankGrandchaserDaily_old.IX_RGRRankGrandchaser_LoginUID
    drop index dbo.RGRRankGrandchaserDaily_old.IXC_RGRRankGrandchaser_TabType

    execute dbo.RGMO_rank_grandchaser_win_lose 1, 0, @bWeb, @sdtDate
    execute dbo.RGMO_rank_grandchaser_win_lose 1, 1, @bWeb, @sdtDate
    execute dbo.RGMO_rank_grandchaser_win_lose 1, 2, @bWeb, @sdtDate
    execute dbo.RGMO_rank_grandchaser_win_lose 1, 3, @bWeb, @sdtDate
    execute dbo.RGMO_rank_grandchaser_win_lose 1, 4, @bWeb, @sdtDate
    execute dbo.RGMO_rank_grandchaser_win_lose 1, 5, @bWeb, @sdtDate
    execute dbo.RGMO_rank_grandchaser_win_lose 1, 6, @bWeb, @sdtDate
    execute dbo.RGMO_rank_grandchaser_win_lose 1, 7, @bWeb, @sdtDate
    execute dbo.RGMO_rank_grandchaser_win_lose 1, 8, @bWeb, @sdtDate

    create clustered index IXC_RGRRankGrandchaser_TabType on dbo.RGRRankGrandchaserDaily_old(TabType, Rank) with fillfactor = 80
    create index IX_RGRRankGrandchaser_Nick on dbo.RGRRankGrandchaserDaily_old(Nick) with fillfactor = 80
    create index IX_RGRRankGrandchaser_LoginUID on dbo.RGRRankGrandchaserDaily_old(LoginUID) with fillfactor = 80

    exec dbo.sp_rename 'RGRRankGrandchaserDaily', 'RGRRankGrandchaserDaily_temp'
    exec dbo.sp_rename 'RGRRankGrandchaserDaily_old', 'RGRRankGrandchaserDaily'
    exec dbo.sp_rename 'RGRRankGrandchaserDaily_temp', 'RGRRankGrandchaserDaily_old'
end


if datepart(week, dateadd(dd, -1, @sdtDate)) + 1 = datepart(week, @sdtDate)
or @bWeekly = 1
begin
    truncate table dbo.RGRRankGrandchaserWeekly_old
    drop index dbo.RGRRankGrandchaserWeekly_old.IX_RGRRankGrandchaser_Nick
    drop index dbo.RGRRankGrandchaserWeekly_old.IX_RGRRankGrandchaser_LoginUID
    drop index dbo.RGRRankGrandchaserWeekly_old.IXC_RGRRankGrandchaser_TabType

    execute dbo.RGMO_rank_grandchaser_win_lose 2, 0, @bWeb, @sdtDate
    execute dbo.RGMO_rank_grandchaser_win_lose 2, 1, @bWeb, @sdtDate
    execute dbo.RGMO_rank_grandchaser_win_lose 2, 2, @bWeb, @sdtDate
    execute dbo.RGMO_rank_grandchaser_win_lose 2, 3, @bWeb, @sdtDate
    execute dbo.RGMO_rank_grandchaser_win_lose 2, 4, @bWeb, @sdtDate
    execute dbo.RGMO_rank_grandchaser_win_lose 2, 5, @bWeb, @sdtDate
    execute dbo.RGMO_rank_grandchaser_win_lose 2, 6, @bWeb, @sdtDate
    execute dbo.RGMO_rank_grandchaser_win_lose 2, 7, @bWeb, @sdtDate
    execute dbo.RGMO_rank_grandchaser_win_lose 2, 8, @bWeb, @sdtDate

    create clustered index IXC_RGRRankGrandchaser_TabType on dbo.RGRRankGrandchaserWeekly_old(TabType, Rank) with fillfactor = 80
    create index IX_RGRRankGrandchaser_Nick on dbo.RGRRankGrandchaserWeekly_old(Nick) with fillfactor = 80
    create index IX_RGRRankGrandchaser_LoginUID on dbo.RGRRankGrandchaserWeekly_old(LoginUID) with fillfactor = 80

    if @bSupply = 1
    begin
    insert into dbo.WIUser
    with (updlock)
    (
        LoginUID
    ,   SupplierUID
    ,   IssueUID
    ,   RegDate
    ,   SupDate
    )
    select
        LoginUID
    ,   1000
    ,   1000 + CharType
    ,   @sdtDate
    ,   @sdtDate
    from dbo.RGRRankGrandchaserWeekly_old
    where
        TabType between 1 and 8
    and Rank between 1 and 3
    option (maxdop 1)
    end

    exec dbo.sp_rename 'RGRRankGrandchaserWeekly', 'RGRRankGrandchaserWeekly_temp'
    exec dbo.sp_rename 'RGRRankGrandchaserWeekly_old', 'RGRRankGrandchaserWeekly'
    exec dbo.sp_rename 'RGRRankGrandchaserWeekly_temp', 'RGRRankGrandchaserWeekly_old'
end


if convert(nvarchar(8), @sdtDate, 120) + N'01' = convert(nvarchar(10), @sdtDate, 120)
or @bMonthly = 1
begin
    truncate table dbo.RGRRankGrandchaserMonthly_old
    drop index dbo.RGRRankGrandchaserMonthly_old.IX_RGRRankGrandchaser_Nick
    drop index dbo.RGRRankGrandchaserMonthly_old.IX_RGRRankGrandchaser_LoginUID
    drop index dbo.RGRRankGrandchaserMonthly_old.IXC_RGRRankGrandchaser_TabType

    execute dbo.RGMO_rank_grandchaser_win_lose 3, 0, @bWeb, @sdtDate
    execute dbo.RGMO_rank_grandchaser_win_lose 3, 1, @bWeb, @sdtDate
    execute dbo.RGMO_rank_grandchaser_win_lose 3, 2, @bWeb, @sdtDate
    execute dbo.RGMO_rank_grandchaser_win_lose 3, 3, @bWeb, @sdtDate
    execute dbo.RGMO_rank_grandchaser_win_lose 3, 4, @bWeb, @sdtDate
    execute dbo.RGMO_rank_grandchaser_win_lose 3, 5, @bWeb, @sdtDate
    execute dbo.RGMO_rank_grandchaser_win_lose 3, 6, @bWeb, @sdtDate
    execute dbo.RGMO_rank_grandchaser_win_lose 3, 7, @bWeb, @sdtDate
    execute dbo.RGMO_rank_grandchaser_win_lose 3, 8, @bWeb, @sdtDate

    create clustered index IXC_RGRRankGrandchaser_TabType on dbo.RGRRankGrandchaserMonthly_old(TabType, Rank) with fillfactor = 80
    create index IX_RGRRankGrandchaser_Nick on dbo.RGRRankGrandchaserMonthly_old(Nick) with fillfactor = 80
    create index IX_RGRRankGrandchaser_LoginUID on dbo.RGRRankGrandchaserMonthly_old(LoginUID) with fillfactor = 80

    if @bSupply = 1
    begin
    insert into dbo.WIUser
    with (updlock)
    (
        LoginUID
    ,   SupplierUID
    ,   IssueUID
    ,   RegDate
    ,   SupDate
    )
    select
        LoginUID
    ,   1000
    ,   1010 + CharType
    ,   @sdtDate
    ,   @sdtDate
    from dbo.RGRRankGrandchaserMonthly_old
    where
        TabType between 1 and 8
    and Rank between 1 and 3
    union all
    select
        LoginUID
    ,   1000
    ,   1020 + CharType
    ,   @sdtDate
    ,   @sdtDate
    from dbo.RGRRankGrandchaserMonthly_old
    where
        TabType between 1 and 8
    and Rank = 1
    union all
    select
        LoginUID
    ,   1000
    ,   1030 + CharType
    ,   @sdtDate
    ,   @sdtDate
    from dbo.RGRRankGrandchaserMonthly_old
    where
        TabType between 1 and 8
    and Rank = 2
    union all
    select
        LoginUID
    ,   1000
    ,   1040 + CharType
    ,   @sdtDate
    ,   @sdtDate
    from dbo.RGRRankGrandchaserMonthly_old
    where
        TabType between 1 and 8
    and Rank = 3
    option (maxdop 1)
    end

    exec dbo.sp_rename 'RGRRankGrandchaserMonthly', 'RGRRankGrandchaserMonthly_temp'
    exec dbo.sp_rename 'RGRRankGrandchaserMonthly_old', 'RGRRankGrandchaserMonthly'
    exec dbo.sp_rename 'RGRRankGrandchaserMonthly_temp', 'RGRRankGrandchaserMonthly_old'
end


if @bExp = 1
begin
    truncate table dbo.RGRRankGrandchaserExp_old
    drop index dbo.RGRRankGrandchaserExp_old.IX_RGRRankGrandchaser_Nick
    drop index dbo.RGRRankGrandchaserExp_old.IX_RGRRankGrandchaser_LoginUID
    drop index dbo.RGRRankGrandchaserExp_old.IXC_RGRRankGrandchaser_TabType

    execute dbo.RGMO_rank_grandchaser_exp 4, 0, @bWeb
    execute dbo.RGMO_rank_grandchaser_exp 4, 1, @bWeb
    execute dbo.RGMO_rank_grandchaser_exp 4, 2, @bWeb
    execute dbo.RGMO_rank_grandchaser_exp 4, 3, @bWeb
    execute dbo.RGMO_rank_grandchaser_exp 4, 4, @bWeb
    execute dbo.RGMO_rank_grandchaser_exp 4, 5, @bWeb
    execute dbo.RGMO_rank_grandchaser_exp 4, 6, @bWeb
    execute dbo.RGMO_rank_grandchaser_exp 4, 7, @bWeb
    execute dbo.RGMO_rank_grandchaser_exp 4, 8, @bWeb

    create clustered index IXC_RGRRankGrandchaser_TabType on dbo.RGRRankGrandchaserExp_old(TabType, Rank) with fillfactor = 80
    create index IX_RGRRankGrandchaser_Nick on dbo.RGRRankGrandchaserExp_old(Nick) with fillfactor = 80
    create index IX_RGRRankGrandchaser_LoginUID on dbo.RGRRankGrandchaserExp_old(LoginUID) with fillfactor = 80

    exec dbo.sp_rename 'RGRRankGrandchaserExp', 'RGRRankGrandchaserExp_temp'
    exec dbo.sp_rename 'RGRRankGrandchaserExp_old', 'RGRRankGrandchaserExp'
    exec dbo.sp_rename 'RGRRankGrandchaserExp_temp', 'RGRRankGrandchaserExp_old'
end


if convert(nvarchar(8), @sdtDate, 120) + N'01' = convert(nvarchar(10), @sdtDate, 120)
begin
    truncate table dbo.RCRRankCharacterAbuse
end


