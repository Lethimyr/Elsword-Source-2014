CREATE PROCEDURE [dbo].[STG_skill_tree_id_update]
	@iLoginUID_ [int],
	@strData1_ [nvarchar](4000) = null,
	@strData2_ [nvarchar](4000) = null,
	@iOK [int] = 0
AS
set nocount on

-- Data : [CharType tinyint,Promotion tinyint,SkillID int,]n

if not exists
(
    select *
    from dbo.Users
    with (nolock)
    where
        LoginUID = @iLoginUID_
)
begin    select @iOK = -1    goto end_proc    end

declare
    @strToken nvarchar(1)
,   @iSplit tinyint
select
    @strToken = N','
,   @iSplit = 3

declare
    @Data1 table
(
    No smallint not null identity(0, 1)
,   Data nvarchar(11) not null
--,   primary key clustered
--    (
--        No
--    )
)
insert into @Data1
(
    Data
)
select
    substring
    (
        @strData1_
    ,   min(No)
    ,   charindex
        (
            @strToken
        ,   @StrData1_ + @strToken
        ,   min(No)
        )
        - min(No)
    )
from dbo.TDNDataNo
with (nolock)
where
    No <= len(@StrData1_)
group by
    charindex
    (
        @strToken
    ,   @StrData1_ + @strToken
    ,   No
    )
order by
    charindex
    (
        @strToken
    ,   @StrData1_ + @strToken
    ,   No
    )
option (maxdop 1)
if @@error <> 0
begin    select @iOK = -2    goto end_proc    end

declare
    @Result1 table
(
    CharType tinyint not null
,   Promotion tinyint not null
,   SkillID int not null
--,   primary key clustered
--    (
--        CharType
--    ,   Promotion
--    ,   SkillID
--    )
)
insert into @Result1
(
    CharType
,   Promotion
,   SkillID
)
select
    max(case when No % @iSplit = 0 then Data end)
,   max(case when No % @iSplit = 1 then Data end)
,   max(case when No % @iSplit = 2 then Data end)
from @Data1
group by
    No / @iSplit
option (maxdop 1)
if @@error <> 0
begin    select @iOK = -3    goto end_proc    end

if exists
(
    select *
    from
    (
        select
            CharType
        from @Result1
        group by
            CharType
    )
    as a
    left outer join
    (
        select
            CharType
        from dbo.STGSkillTreeSP
        with (nolock)
        where
            LoginUID = @iLoginUID_    
    )
    as b
    on
        a.CharType = b.CharType
    where
        b.CharType is null
)
begin    select @iOK = -4    goto end_proc    end

if exists
(
    select *
    from dbo.STGSkillTreeID
    as a
    with (nolock)
    join @Result1
    as b
    on
        a.CharType = b.CharType
    and a.Promotion = b.Promotion
    and a.SkillID = b.SkillID
    where
        a.LoginUID = @iLoginUID_
)
begin    select @iOK = -5    goto end_proc    end

declare
    @Data2 table
(
    No smallint not null identity(0, 1)
,   Data nvarchar(11) not null
--,   primary key clustered
--    (
--        No
--    )
)
insert into @Data2
(
    Data
)
select
    substring
    (
        @strData2_
    ,   min(No)
    ,   charindex
        (
            @strToken
        ,   @StrData2_ + @strToken
        ,   min(No)
        )
        - min(No)
    )
from dbo.TDNDataNo
with (nolock)
where
    No <= len(@StrData2_)
group by
    charindex
    (
        @strToken
    ,   @StrData2_ + @strToken
    ,   No
    )
order by
    charindex
    (
        @strToken
    ,   @StrData2_ + @strToken
    ,   No
    )
option (maxdop 1)
if @@error <> 0
begin    select @iOK = -6    goto end_proc    end

declare
    @Result2 table
(
    CharType tinyint not null
,   Promotion tinyint not null
,   SkillID int not null
--,   primary key clustered
--    (
--        CharType
--    ,   Promotion
--    ,   SkillID
--    )
)
insert into @Result2
(
    CharType
,   Promotion
,   SkillID
)
select
    max(case when No % @iSplit = 0 then Data end)
,   max(case when No % @iSplit = 1 then Data end)
,   max(case when No % @iSplit = 2 then Data end)
from @Data2
group by
    No / @iSplit
option (maxdop 1)
if @@error <> 0
begin    select @iOK = -7    goto end_proc    end

if exists
(
    select *
    from @Result2
    as a
    left outer join
    (
        select
            CharType
        ,   Promotion
        ,   SkillID
        from dbo.STGSkillTreeID
        with (nolock)
        where
            LoginUID = @iLoginUID_
    )
    as b
    on
        a.CharType = b.CharType
    and a.Promotion = b.Promotion
    and a.SkillID = b.SkillID
    where
        b.CharType is null
)
begin    select @iOK = -8    goto end_proc    end

declare
    @sdtDate smalldatetime
select
    @sdtDate = getdate()

begin tran
    delete a
    with (updlock)
    from dbo.STGSkillTreeID
    as a
    with (updlock)
    join @Result2
    as b
    on
        a.CharType = b.CharType
    and a.Promotion = b.Promotion
    and a.SkillID = b.SkillID
    where
        a.LoginUID = @iLoginUID_
    option (maxdop 1)
    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end

    insert into dbo.STGSkillTreeID
    with (updlock)
    (
        LoginUID
    ,   CharType
    ,   Promotion
    ,   SkillID
    )
    select
        @iLoginUID_
    ,   CharType
    ,   Promotion
    ,   SkillID
    from @Result1
    option (maxdop 1)
    if @@error <> 0
    begin    select @iOK = -102    goto fail_tran    end

    insert into dbo.STLSkillTreeID
    with (updlock)
    (
        LoginUID
    ,   CharType
    ,   Promotion
    ,   SkillID
    ,   Action
    ,   RegDateB
    )
    select
        @iLoginUID_
    ,   CharType
    ,   Promotion
    ,   SkillID
    ,   1 --insert
    ,   @sdtDate
    from @Result1
    union all
    select
        @iLoginUID_
    ,   CharType
    ,   Promotion
    ,   SkillID
    ,   2 --delete
    ,   @sdtDate
    from @Result2
    option (maxdop 1)
    if @@error <> 0
    begin    select @iOK = -103    goto fail_tran    end
commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:
select
    @iOK


