CREATE PROCEDURE [dbo].[gei_equip_item_update]
	@iLoginUID_ [int],
	@strEquipItem_ [nvarchar](4000),
	@iOK [int] = 0,
	@iError [int] = 0
AS
set nocount on;

declare @strLogin nvarchar(20)
select  @strLogin = Login
    from dbo.Users with (nolock)
        where LoginUID = @iLoginUID_
if @strLogin is null
    begin    select @iOK = -1    goto end_proc    end

declare
    @result table
(
    No smallint not null identity(0, 1)
,   CharType tinyint not null
,   Data nvarchar(4000) not null
--,   primary key clustered
--    (
--        No
--    )
)

declare @strToken1 nvarchar(1)
,       @strToken2 nvarchar(1)
,       @iStart int
,       @iEnd int
,       @iSplit tinyint
,       @iCharType tinyint
,       @strData nvarchar(4000)
select  @strToken1 = N';'
,       @strToken2 = N','
,       @iStart = 1
,       @iEnd = 1
,       @iSplit = 3

while 1 = 1
    begin
        select @iEnd = charindex(@strToken1, @strEquipItem_, @iStart + 1)
        if @iEnd = 0
            begin    break    end

        select @strData = substring(@strEquipItem_, @iStart, @iEnd - @iStart)

        select @iCharType = substring(@strData, 1, charindex(@strToken2, @strData, 1) - 1)

        select @strData = substring(@strData, 1 + charindex(@strToken2, @strData, 1), 4000)

        insert into
            @result
        (
            CharType
,           Data
        )
        select
            @iCharType
,           substring(@strData, min(a.No), charindex(@strToken2, @strData + @strToken2, min(a.No)) - min(a.No))
        from dbo.TDNDataNo as a with (nolock)
        where a.No <= len(@strData)
        group by charindex(@strToken2, @strData + @strToken2, a.No)
        order by charindex(@strToken2, @strData + @strToken2, a.No)

        select @iStart = @iEnd + 1
    end

declare @EquipItem table
(
    CharType tinyint not null
,   ItemUID int not null
,   ItemID int not null
,   ItemType bit not null
--,   primary key clustered
--    (
--        CharType
--    ,   ItemUID
--    ,   ItemID
--    ,   ItemType
--    )
)

insert into
    @EquipItem
(
    CharType
,   ItemUID
,   ItemID
,   ItemType
)
select
    CharType
,   max(case when No % @iSplit = 0 then Data end)
,   max(case when No % @iSplit = 1 then Data end)
,   max(case when No % @iSplit = 2 then Data end)
from @result
group by CharType, No / @iSplit
if @@error <> 0
    begin    select @iOK = -2, @iError = @@error    goto end_proc    end

if exists
(
    select *
    from @EquipItem as a
    left outer join dbo.CPCharacter as b with (nolock)
    on a.CharType = b.CharType
    where b.CharType is null
)
    begin    select @iOK = -3    goto end_proc    end

if exists
(
    select *
    from @EquipItem as a
    left outer join dbo.GoodsObjectList as b with (nolock)
    on a.ItemUID = b.ItemUID
    and a.ItemID = b.ItemID
    where b.ItemUID is null
    and b.OwnerLogin = @strLogin
    and a.ItemType = 0
)
    begin    select @iOK = -4    goto end_proc    end

if exists
(
    select *
    from @EquipItem as a
    left outer join dbo.DurationItemObjectList as b with (nolock)
    on a.ItemUID = b.DurationItemUID
    and a.ItemID = b.GoodsID
    where b.DurationItemUID is null
    and b.OwnerLogin = @strLogin
    and a.ItemType = 1
)
    begin    select @iOK = -5    goto end_proc    end

begin tran
    delete a
        from dbo.GEIEquipItem as a with (updlock)
            where LoginUID = @iLoginUID_
    if @@error <> 0
        begin    select @iOK = -11, @iError = @@error    goto fail_tran    end

    insert into dbo.GEIEquipItem with (updlock)
            (
                LoginUID
,               CharType
,               ItemUID
,               ItemID
,               ItemType
            )
        select  @iLoginUID_
,               CharType
,               ItemUID
,               ItemID
,               ItemType
            from @EquipItem
    if @@error <> 0
        begin    select @iOK = -12, @iError = @@error    goto fail_tran    end
commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:
select @iOK


