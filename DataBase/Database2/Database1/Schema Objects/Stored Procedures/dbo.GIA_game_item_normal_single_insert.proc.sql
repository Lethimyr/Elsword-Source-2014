CREATE PROCEDURE [dbo].[GIA_game_item_normal_single_insert]
	@iLoginUID_ [int],
	@iItemID_ [int],
	@iFactor_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20080910. microcat.
*/

declare
    @iItemUID int
,   @iFactor int
,   @sdtRegDateB smalldatetime
,   @sdtStartDateB smalldatetime
,   @sdtEndDateB smalldatetime
,   @strBuyerLogin nvarchar(20)
select
    @iItemUID = -1
,   @iFactor = @iFactor_
,   @sdtRegDateB = current_timestamp
,   @sdtStartDateB = @sdtRegDateB
,   @sdtEndDateB = dateadd(dd, @iFactor_, @sdtRegDateB)
,   @strBuyerLogin = N'__mc__' + convert(nvarchar(8), @sdtRegDateB, 112) + N'__99__'

declare
    @strLogin nvarchar(20)
execute dbo.AAA_login_check
    @iLoginUID_
,   @iOK output
,   @strLogin output
if @iOK <> 0
begin    select @iOK = -1    goto end_proc    end

execute dbo.AAA_item_exist
    @iItemID_
,   0
,   0
,   @iOK output
if @iOK <> 0
begin    select @iOK = -2    goto end_proc    end

if @iFactor > 0
and exists
(
    select *
    from dbo.GoodsObjectList
    where
        OwnerLogin = @strLogin
    and ItemID = @iItemID_
    and Period > 0
)
begin
    begin transaction

    update a
    with (updlock)
    set
        @iItemUID = a.ItemUID
    ,   a.BuyerLogin = @strBuyerLogin
    ,   @sdtRegDateB = a.RegDate
    ,   @sdtStartDateB = a.StartDate
    ,   @sdtEndDateB = a.EndDate = dateadd(dd, @iFactor, a.EndDate)
    ,   a.Period = a.Period + @iFactor
    from dbo.GoodsObjectList
    as a
    with (updlock)
    where
        OwnerLogin = @strLogin
    and ItemID = @iItemID_
    and Period > 0
    option (maxdop 1)

    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end
end
else
begin
    begin transaction

    insert into dbo.GoodsObjectList
    with (updlock)
    (
        OwnerLogin
    ,   BuyerLogin
    ,   ItemID
    ,   RegDate
    ,   StartDate
    ,   EndDate
    ,   Period
    )
    select
        @strLogin
    ,   @strBuyerLogin
    ,   @iItemID_
    ,   @sdtRegDateB
    ,   @sdtStartDateB
    ,   @sdtEndDateB
    ,   @iFactor
    option (maxdop 1)

    if @@error <> 0
    begin    select @iOK = -102    goto fail_tran    end

    select
        @iItemUID = @@identity
end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
select
    @iOK
,   @iItemUID
,   @iFactor
,   @sdtRegDateB
,   @sdtStartDateB
,   @sdtEndDateB
,   @strBuyerLogin
option (maxdop 1)


