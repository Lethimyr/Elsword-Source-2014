CREATE PROCEDURE [dbo].[DSG_divide_sell_item]
	@iLoginUID_ [int],
	@iItemUID_ [int],
	@iItemID_ [int],
	@iFactor_ [int],
	@iOK [int] = 0,
	@iItemUID [int] = 0,
	@iItemID [int] = 0,
	@fRValue [float] = 0,
	@fRValueDiff [float] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20080508. microcat.
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
,   @sdtRegDate smalldatetime
,   @iFactor int
select
    @strBuyerLogin = BuyerLogin
,   @iFactor = Duration
,   @sdtRegDate = RegDate
from dbo.DurationItemObjectList
where
    DurationItemUID = @iItemUID_
and OwnerLogin = @strLogin
and GoodsID = @iItemID_
and Duration >= @iFactor_

if @strBuyerLogin is null
begin    select @iOK = -2    goto end_proc    end

declare
    @iDivideFactor int
,   @iCashPerGem int
select
    @iDivideFactor = Variable1
,   @iCashPerGem = Variable2
from dbo.GlobalVariables
where
    No = 4

declare
    @iType tinyint
,   @fPrice float
,   @fFactor float
select
    @iType = Type
,   @fPrice =
        case
            when Factor >= Price then case when Price = 0 or Factor = 0 then 0 else 1 end
            when Factor < Price then case when Price = 0 or Factor = 0 then 0 else Price / Factor end
        end
,   @fFactor =
        case
            when Factor >= Price then case when Price = 0 or Factor = 0 then 0 else Factor / Price end
            when Factor < Price then case when Price = 0 or Factor = 0 then 0 else 1 end
        end
from
(
    select
        case
            when Cash = 1 or Gem > 0 then 2 --잼
            when Crystal > 0 then 3 --크리스탈
            else 1 --GP
        end as Type
    ,   Duration as Factor
    ,   case
            when Gem > 0 then Gem
            when Crystal > 0 then Crystal
            else Price
        end
        /
        case Cash
            when 1 then isnull(@iCashPerGem, 200.0)
            else @iDivideFactor
        end as Price
    from dbo.GoodsInfoList
    where
        ItemType = 1
    and GoodsID = @iItemID_
) as a
if @iType is null
begin    select @iOK = -3    goto end_proc    end

select
    @fRValueDiff =
        case
            when @iFactor_ = 0 or @fPrice = 0 or @fFactor = 0 then 0
            else @iFactor_ * @fPrice / @fFactor
        end

if @fRValueDiff >= 1
begin
    if @iType = 1
    begin
        select
            @fRValue = dbo.zcGamePoint(GamePoint + @fRValueDiff)
        from dbo.Users
        where
            Login = @strLogin
        and LoginUID = @iLoginUID_
    end
    else if @iType between 2 and 3
    begin
        select
            @iItemID =
                case @iType
                    when 2 then 8880
                    when 3 then 12490
                end

        select
            @iItemUID = DurationitemUID
        ,   @fRValue = Duration + @fRValueDiff
        from dbo.DurationItemObjectList
        where
            OwnerLogin = @strLogin
        and GoodsID = @iItemID

        if @iItemUID = 0
        begin
            begin transaction
                insert into dbo.DurationItemObjectList
                with (updlock)
                (
                    OwnerLogin
                ,   BuyerLogin
                ,   GoodsID
                ,   Duration
                )
                select
                    @strLogin
                ,   N'__mc__20080508__01__'
                ,   @iItemID
                ,   0

                if @@error <> 0
                begin    select @iOK = -101    goto fail_tran    end

                select
                    @iItemUID = @@identity
                ,   @fRValue = @fRValueDiff
            commit transaction
        end
    end
end

begin transaction
    if @fRValueDiff >= 1
    begin
        if @iType = 1
        begin
            update a
            with (updlock)
            set
                a.GamePoint = dbo.zcGamePoint(a.GamePoint + @fRValueDiff)
            from dbo.Users
            as a
            with (updlock)
            where
                Login = @strLogin
            and LoginUID = @iLoginUID_

            if @@error <> 0
            begin    select @iOK = -102    goto fail_tran    end
        end
        else if @iType between 2 and 3
        begin
            update a
            with (updlock)
            set
                a.Duration = a.Duration + @fRValueDiff
            from dbo.DurationItemObjectList
            as a
            with (updlock)
            where
                OwnerLogin = @strLogin
            and GoodsID = @iItemID

            if @@error <> 0
            begin    select @iOK = -103    goto fail_tran    end
        end
        else
        begin    select @iOK = -104    goto fail_tran    end
    end

    if @iFactor_ = @iFactor
    begin
        delete a
        with (updlock)
        from dbo.DurationItemObjectList
        as a
        with (updlock)
        where
            DurationItemUID = @iItemUID_
        and OwnerLogin = @strLogin
        and GoodsID = @iItemID_
        and Duration = @iFactor_

        if @@error <> 0
        begin    select @iOK = -105    goto fail_tran    end

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
        ,   @sdtRegDate
        ,   current_timestamp

        if @@error <> 0
        begin    select @iOK = -106    goto fail_tran    end
    end
    else
    begin
        update a
        with (updlock)
        set
            a.Duration = a.Duration - @iFactor_
        from dbo.DurationItemObjectList
        as a
        with (updlock)
        where
            DurationItemUID = @iItemUID_
        and OwnerLogin = @strLogin
        and GoodsID = @iItemID_
        and Duration >= @iFactor_

        if @@error <> 0
        begin    select @iOK = -107    goto fail_tran    end
    end

execute [Log].dbo.CIMO_count_item_change_insert
    @iLoginUID_
,   @iItemUID_
,   @iItemID_
,   1
,   @iFactor_
,   @iOK output

if @iOK <> 0
begin    select @iOK = -108    goto fail_tran    end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
select
    @iOK
,   @iItemUID
,   @iItemID
,   @fRValue
,   @fRValueDiff


