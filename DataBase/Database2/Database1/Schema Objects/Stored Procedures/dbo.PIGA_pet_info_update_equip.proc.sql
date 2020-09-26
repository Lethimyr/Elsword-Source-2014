create PROCEDURE [dbo].[PIGA_pet_info_update_equip]
	@iLoginUID_ [int],
	@strData_ [nvarchar](4000),
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081024. microcat.

{ call dbo.PIGA_pet_info_update_equip ( %d, N'%s' ) }

{ call dbo.PIGA_pet_info_update_equip ( @1, @2 ) }
@1 ; LoginUID int
@2 ; Data nvarchar(4000) ; [@21,@22,@23,]n
    @21 ; PetUID int
    @22 ; ItemUID int
    @23 ; ItemType int

1 return ( @1 )
@1 ; OK int
*/

execute dbo.AAA_login_check
    @iLoginUID_
,   @iOK output

if @iOK <> 0
begin    select @iOK = -1    goto end_proc    end

declare
    @strToken nvarchar(1)
select
    @strToken = N','

while right(@strData_, 1) = @strToken
begin
    select
        @strData_ = SubString(@strData_, 1, Len(@strData_) - 1)
end

declare
    @iStart int
,   @iEnd int
,   @iPetUID int
,   @iItemUID int
,   @iItemType int
select
    @iStart = 1
,   @iEnd = 1

declare
    @Result table
(
    PetUID int not null
,   ItemUID int not null
,   ItemType int not null
,   ItemID int not null
,   HackDetection bit not null
)

while 1 = 1
begin
    select
        @iEnd = charindex(@strToken, @strData_, @iStart + 1)
    if @iEnd = 0
    begin    select @iOK = -2    goto end_proc    end
    select
        @iPetUID = substring(@strData_, @iStart, @iEnd - @iStart)

    select
        @iStart = @iEnd + 1

    select
        @iEnd = charindex(@strToken, @strData_, @iStart + 1)
    if @iEnd = 0
    begin    select @iOK = -3    goto end_proc    end
    select
        @iItemUID = substring(@strData_, @iStart, @iEnd - @iStart)

    select
        @iStart = @iEnd + 1

    select
        @iEnd = CharIndex(@strToken, @strData_, @iStart + 1)
    if @iEnd = 0
    begin    select @iEnd = 4000    end
    select
        @iItemType = substring(@strData_, @iStart, @iEnd - @iStart)

    if exists
    (
        select *
        from @Result
        where
            PetUID = @iPetUID
        and ItemUID = @iItemUID
        and ItemType = @iItemType
    )
    begin    select @iOK = -4    goto end_proc    end

--kind값(장착 아이템은 kind값 52와 53번이어야 한다.)
--50 펫(38810 루나, 38820 페페, 38830 시트, 38840 블루페어리)
--51 펫먹이(8880 잼, 12490 크리스탈, 12520 빛을 잃은 크리스탈, 38850 펫용 사료, 38860 고급 사료)
--52 펫기간장착아이템(38870 영양 주사, 38880 영양 링거)
--53 펫수량장착아이템(38890/38900 스크래치, 38910/38920 아이스애로우, 38930/38940 헤드봄, 38950/39860 윈디필로우)

    declare
        @iItemID int
    select
        @iItemID = null

    if @iItemType = 1
    begin
        select
            @iItemID = GoodsID
        from dbo.DurationItemObjectList
        where
            DurationItemUID = @iItemUID
        option (maxdop 1)
    end
    else if @iItemType = 0
    begin
        select
            @iItemID = ItemID
        from dbo.GoodsObjectList
        where
            ItemUID = @iItemUID
        option (maxdop 1)
    end
    else
    begin    select @iOK = -5    goto end_proc    end

    if @iItemID is not null
    begin
        declare
            @bHackDetection bit
        select
            @bHackDetection = 0 --전제는 해킹가능성 없음
        if not exists
        (
            select *
            from dbo.GoodsInfoList
            where
                GoodsID = @iItemID
            AND Kind = 52 + @iItemType
        )
        begin
            select
                @bHackDetection = 1
        end

        insert into @Result
        (
            PetUID
        ,   ItemUID
        ,   ItemType
        ,   ItemID
        ,   HackDetection
        )
        select
            @iPetUID
        ,   @iItemUID
        ,   @iItemType
        ,   @iItemID
        ,   @bHackDetection
    end

    if @iEnd = 4000
    begin    break    end

    select
        @iStart = @iEnd + 1
end

begin transaction

delete a
with (updlock)
from dbo.GPetEquip
as a
with (updlock)
left outer join
(
    select
        PetUID
    ,   ItemUID
    from @Result
    where
        HackDetection = 0
)
as b
on
    a.PetUID = b.PetUID
and a.ItemUID = b.ItemUID
join
(
    select
        PetUID
    from dbo.GPet
    where
        LoginUID = @iLoginUID_
)
as c
on
    a.PetUID = c.PetUID
where
    b.PetUID is null
option (maxdop 1)

if @@error <> 0
begin    select @iOK = -101    goto fail_tran    end

insert into dbo.GPetEquip
with (updlock)
(
    PetUID
,   ItemUID
,   ItemType
)
select
    a.PetUID
,   a.ItemUID
,   a.ItemType
from
(
    select
        PetUID
    ,   ItemUID
    ,   ItemType
    from @Result
    where
        HackDetection = 0
)
as a
left outer join dbo.GPetEquip
as b
with (updlock)
on
    a.PetUID = b.PetUID
and a.ItemUID = b.ItemUID
join
(
    select
        PetUID
    from dbo.GPet
    where
        LoginUID = @iLoginUID_
)
as c
on
    a.PetUID = c.PetUID
where
    b.PetUID is null
option (maxdop 1)

if @@error <> 0
begin    select @iOK = -102    goto fail_tran    end

insert into dbo.GPetEquipHackDetection
with (updlock)
(
    LoginUID
,   PetUID
,   ItemUID
,   ItemType
,   ItemID
)
select
    @iLoginUID_
,   PetUID
,   ItemUID
,   ItemType
,   ItemID
from @Result
where
    HackDetection = 1
option (maxdop 1)

if @@error <> 0
begin    select @iOK = -103    goto fail_tran    end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
select
    @iOK


