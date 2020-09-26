CREATE PROCEDURE [dbo].[GIA_game_item_duration_single_insert]
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
,   @strBuyerLogin nvarchar(20)
select
    @iItemUID = -1
,   @iFactor = @iFactor_
,   @sdtRegDateB = current_timestamp
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
,   1
,   @iOK output
if @iOK <> 0
begin    select @iOK = -2    goto end_proc    end

if exists
(
    select *
    from dbo.DurationItemObjectList
    where
        OwnerLogin = @strLogin
    and GoodsID = @iItemID_
)
begin
    begin transaction

    update a
    with (updlock)
    set
        @iItemUID = a.DurationItemUID
    ,   a.BuyerLogin = @strBuyerLogin
    ,   a.Duration = a.Duration + @iFactor
    from dbo.DurationItemObjectList
    as a
    with (updlock)
    where
        OwnerLogin = @strLogin
    and GoodsID = @iItemID_
    option (maxdop 1)

    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end
end
else
begin
    begin transaction

    insert into dbo.DurationItemObjectList
    with (updlock)
    (
        OwnerLogin
    ,   BuyerLogin
    ,   GoodsID
    ,   Duration
    ,   RegDate
    )
    select
        @strLogin
    ,   @strBuyerLogin
    ,   @iItemID_
    ,   @iFactor
    ,   @sdtRegDateB
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
,   @strBuyerLogin
option (maxdop 1)


