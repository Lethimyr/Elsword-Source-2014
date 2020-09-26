CREATE PROCEDURE [dbo].[CIGA_count_item_update]
	@iLoginUID_ [int],
	@iItemUID_ [int],
	@iItemID_ [int],
	@iFactor_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20080918. microcat.
*/

declare
    @strLogin nvarchar(20)
execute dbo.AAA_login_check
    @iLoginUID_
,   @iOK output
,   @strLogin output

if @iOK <> 0
begin    select @iOK = -1    goto end_proc    end

declare
    @strBuyerLogin nvarchar(20)
,   @bU0D1 bit
,   @sdtRegDateB smalldatetime
select
    @strBuyerLogin = BuyerLogin
,   @bU0D1 = case when @iFactor_ < Duration then 0 else 1 end
,   @sdtRegDateB = RegDate
from dbo.DurationitemObjectList
where
    DurationitemUID = @iItemUID_
and OwnerLogin = @strLogin
and GoodsID = @iItemID_
option (maxdop 1)

if @strBuyerLogin is null
begin    select @iOK = -2    goto end_proc    end

if @bU0D1 = 0
begin
    begin transaction

    update a
    with (updlock)
    set
        a.Duration = a.Duration - @iFactor_
    from dbo.DurationItemObjectList
    as a
    with (updlock)
    where
        a.DurationItemUID = @iItemUID_
    and a.OwnerLogin = @strLogin
    and a.GoodsID = @iItemID_
    option (maxdop 1)

    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end
end
else if @bU0D1 = 1
begin
    begin transaction

    delete a
    with (updlock)
    from dbo.DurationItemObjectList
    as a
    with (updlock)
    where
        a.DurationItemUID = @iItemUID_
    and a.OwnerLogin = @strLogin
    and a.GoodsID = @iItemID_
    option (maxdop 1)

    if @@error <> 0
    begin    select @iOK = -102    goto fail_tran    end

    insert into [Log].dbo.ICLOItemCountItem
    with (updlock)
    (
        LoginUID
    ,   ItemUID
    ,   BuyerLogin
    ,   ItemID
    ,   RegDate
    ,   DelDate
    )
    select
        @iLoginUID_
    ,   @iItemUID_
    ,   @strBuyerLogin
    ,   @iItemID_
    ,   @sdtRegDateB
    ,   current_timestamp

    if @@error <> 0
    begin    select @iOK = -103    goto fail_tran    end
end

execute [Log].dbo.CIMO_count_item_change_insert
    @iLoginUID_
,   @iItemUID_
,   @iItemID_
,   2
,   @iFactor_
,   @iOK output

if @iOK <> 0
begin    select @iOK = -104    goto fail_tran    end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
select
    @iOK


