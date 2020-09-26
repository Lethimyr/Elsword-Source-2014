CREATE PROCEDURE [dbo].[STG_skill_tree_group_update]
	@iLoginUID_ [int],
	@strData_ [nvarchar](4000),
	@iOK [int] = 0
AS
set nocount on

-- Data : [CharType tinyint,GroupID int,]n

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
,   @iSplit = 2

declare
    @Data table
(
    No smallint not null identity(0, 1)
,   Data nvarchar(11) not null
--,   primary key clustered
--    (
--        No
--    )
)
insert into @Data
(
    Data
)
select
    substring
    (
        @strData_
    ,   min(No)
    ,   charindex
        (
            @strToken
        ,   @strData_ + @strToken
        ,   min(No)
        )
        - min(No)
    )
from dbo.TDNDataNo
with (nolock)
where
    No <= len(@strData_)
group by
    charindex
    (
        @strToken
    ,   @strData_ + @strToken
    ,   No
    )
order by
    charindex
    (
        @strToken
    ,   @strData_ + @strToken
    ,   No
    )
option (maxdop 1)
if @@error <> 0
begin    select @iOK = -2    goto end_proc    end

declare
    @Result table
(
    CharType tinyint not null
,   GroupID int not null
--,   primary key clustered
--    (
--        CharType
--    ,   GroupID
--    )
)
insert into @Result
(
    CharType
,   GroupID
)
select
    max(case when No % @iSplit = 0 then Data end)
,   max(case when No % @iSplit = 1 then Data end)
from @Data
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
        from @Result
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
    from @Result
    as a
    join dbo.STGSkillTreeGroup
    as b
    with (nolock)
    on
        a.CharType = b.CharType
    and a.GroupID = b.GroupID
    where
        b.LoginUID = @iLoginUID_
)
begin    select @iOK = -5    goto end_proc    end

begin tran
    insert into dbo.STGSkillTreeGroup
    with (updlock)
    (
        LoginUID
    ,   CharType
    ,   GroupID
    )
    select
        @iLoginUID_
    ,   CharType
    ,   GroupID
    from @Result
    option (maxdop 1)
    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end
commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:
select
    @iOK


