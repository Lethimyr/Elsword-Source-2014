CREATE PROCEDURE [dbo].[SIG_strong_item]
	@strLogin_ [nvarchar](20),
	@iLoginUID_ [int],
	@iItemUID_ [int],
	@iItemID_ [int],
	@iDecision_ [int],
	@iGemUseage_ [int],
	@iOK [int] = 0,
	@iItemID [int] = -1
AS
set nocount on

if not exists
(
    select *
    from dbo.Users
    with (nolock)
    where
        Login = @strLogin_
    and LoginUID = @iLoginUID_
)
begin    select @iOK = -1    goto end_proc    end

if not exists
(
    select *
    from dbo.GoodsObjectList
    with (nolock)
    where
        ItemUID = @iItemUID_
    and OwnerLogin = @strLogin_
    and ItemID = @iItemID_
)
begin    select @iOK = -2    goto end_proc    end

if not exists
(
    select *
    from dbo.DurationItemObjectList
    with (nolock)
    where
        OwnerLogin = @strLogin_
    and GoodsID = 8880
    and Duration >= @iGemUseage_
)
begin    select @iOK = -3    goto end_proc    end

declare
    @iCharType tinyint
,   @iKind tinyint
,   @iLevel tinyint
,   @bUnableStrong bit
select
    @iCharType = CharType
,   @iKind = Kind
,   @iLevel = Level
,   @bUnableStrong = UnableStrong
from dbo.SIGStrongItem
with (nolock)
where
    ItemID = @iItemID_
and UnableStrong = 0
if @iCharType is null
begin    select @iOK = -4    goto end_proc    end

if not exists
(
    select *
    from dbo.GoodsInfoList
    with (nolock)
    where
        GoodsID = @iItemID_
    and CharType = @iCharType
    and StrongItemKind = @iKind
    and StrongLevel = @iLevel
    and UnableStrong = @bUnableStrong
)
begin    select @iOK = -5    goto end_proc    end

select
    @iItemID = ItemID
,   @iCharType = CharType
,   @iKind = Kind
,   @iLevel = Level
,   @bUnableStrong = UnableStrong
from dbo.SIGStrongItem
with (nolock)
where
    CharType = @iCharType
and Kind = @iKind
and Level = @iLevel +
    case @iDecision_
        when 0 then 1
        when 1 then -1
        when 2 then -5
        when 3 then 0
    end
and
(
    UnableStrong =
        case @iDecision_
            when 2 then 1
            else 0
        end
or  Level = 9
)
if @iItemID = -1
begin    select @iOK = -6    goto end_proc    end

if not exists
(
    select *
    from dbo.GoodsInfoList
    with (nolock)
    where
        GoodsID = @iItemID
    and CharType = @iCharType
    and StrongItemKind = @iKind
    and StrongLevel = @iLevel
    and UnableStrong = @bUnableStrong
)
begin    select @iOK = -7    goto end_proc    end

begin tran
    update a
    with (updlock)
    set
        a.ItemID = @iItemID
    from dbo.GoodsObjectList
    as a
    with (updlock)
    where
        ItemUID = @iItemUID_
    and OwnerLogin = @strLogin_
    and ItemID = @iItemID_
    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end

    update a
    with (updlock)
    set
        a.Duration = a.Duration - @iGemUseage_
    from dbo.DurationitemObjectList
    as a
    with (updlock)
    where
        OwnerLogin = @strLogin_
    and GoodsID = 8880
    and Duration >= @iGemUseage_
    if @@error <> 0
    begin    select @iOK = -102    goto fail_tran    end

    insert into dbo.SILStrongItem
    with (updlock)
    (
        LoginUID
    ,   ItemUID
    ,   OldItemID
    ,   NewItemID
    ,   RegDate
    )
    select
        @iLoginUID_
    ,   @iItemUID_
    ,   @iItemID_
    ,   @iItemID
    ,   getdate()
    if @@error <> 0
    begin    select @iOK = -103    goto fail_tran    end
commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:
select
    @iOK
,   @iItemID


