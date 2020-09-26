CREATE PROCEDURE [dbo].[gup_strong_item_enchant]
	@iLoginUID_ [int],
	@iStuffItemUID_ [int],
	@iStuffItemID_ [int],
	@iStuffItemCount_ [int],
	@iAssistanceItemUID_ [int],
	@iAssistanceItemID_ [int],
	@iStrongItemUID_ [int],
	@iStrongItemID_ [int],
	@bBroken_ [bit],
	@iStrongLevel_ [tinyint] = 0,
	@iOK [int] = 0
AS
set nocount on;

--Stuff         : 수량, 재료 아이템(라그나로크의 오리, 에르), 입력 받은만큼 차감
--Assistance    : 수량, 보조 아이템(요구르팅의 교과서), 1회 호출시 1개 차감
--Strong        : 영구, 실제 강화될 아이템

--테이블 변수 정의
declare @Item table
(
    No int not null
,   ItemUID int not null
,   BuyerLogin nvarchar(20) null
,   ItemID int not null
,   RegDate smalldatetime null
,   StartDate smalldatetime null
,   EndDate smalldatetime null
,   Factor int not null
--,   primary key clustered
--    (
--        No
--    )
)

--유저 확인
declare @strLogin nvarchar(20)
select  @strLogin = Login
    from dbo.Users with (nolock)
        where LoginUID = @iLoginUID_
if @strLogin is null
    begin    select @iOK = -1    goto end_proc    end

--재료 아이템 확인
insert into @Item
        (
            No
,           ItemUID
,           ItemID
,           Factor
        )
    select  1
,           DurationItemUID
,           GoodsID
,           Duration - @iStuffItemCount_
        from dbo.DurationItemObjectList with (nolock)
            where DurationItemUID = @iStuffItemUID_
            and OwnerLogin = @strLogin
            and GoodsID = @iStuffItemID_
            and @iStuffItemCount_ <= Duration
if not exists
    (
        select *
            from @Item
                where No = 1
    )
    begin    select @iOK = -2    goto end_proc    end

--보조 아이템 확인
insert into @Item
        (
            No
,           ItemUID
,           ItemID
,           Factor
        )
    select  2
,           DurationItemUID
,           GoodsID
,           Duration - 1
        from dbo.DurationItemObjectList with (nolock)
            where DurationItemUID = @iAssistanceItemUID_
            and OwnerLogin = @strLogin
            and GoodsID = @iAssistanceItemID_
            and 1 <= Duration
            and @iAssistanceItemID_ <> 0xffffffff
--보조 아이템은 입력이 안될 수도 있으므로 존재 유무를 체크하지 않는다.

--강화 아이템 리스트 확인
if not exists
    (
        select *
            from dbo.GStrongItemInfo with (nolock)
                where ItemID = @iStrongItemID_
    )
    begin    select @iOK = -3    goto end_proc    end

--강화 아이템 확인
insert into @Item
        (
            No
,           ItemUID
,           BuyerLogin
,           ItemID
,           RegDate
,           StartDate
,           EndDate
,           Factor
        )
    select  3
,           ItemUID
,           BuyerLogin
,           ItemID
,           RegDate
,           StartDate
,           EndDate
,           Period
        from dbo.GoodsObjectList with (nolock)
            where ItemUID = @iStrongItemUID_
            and OwnerLogin = @strLogin
            and ItemID = @iStrongItemID_
            and Period = -1
if not exists
    (
        select *
            from @Item
                where No = 3
    )
    begin    select @iOK = -4    goto end_proc    end

--강화 레벨 확인
declare @iStrongLevel tinyint
select  @iStrongLevel = StrongLevel
    from dbo.GStrongItem with (nolock)
        where ItemUID = @iStrongItemUID_

--강화 액션
declare @iActionType tinyint
select  @iActionType = 1
if @iStrongLevel is null
    begin
        select  @iActionType = 0 --첫강화
,               @iStrongLevel = 0
    end
if @bBroken_ = 1
    begin
        select @iActionType = 2 --깨졌다
    end

--트랜잭션
declare @dtDate datetime
select  @dtDate = getdate()
begin tran
    --FK_GStrongItem_GoodsObjectList
    delete a with (updlock)
        from dbo.GoodsObjectList as a with (updlock)
        join @Item as b
            on a.ItemUID = b.ItemUID
            and a.OwnerLogin = @strLogin
            and a.ItemID = b.ItemID
            and b.No = 3
            and @iActionType = 2
    if @@error <> 0
        begin    select @iOK = -101    goto fail_tran    end

    update a with (updlock)
        set a.Duration = b.Factor
            from dbo.DurationItemObjectList as a with (updlock)
            join @Item as b
                on a.DurationItemUID = b.ItemUID
                and a.OwnerLogin = @strLogin
                and a.GoodsID = b.ItemID
                and b.No between 1 and 2
    if @@error <> 0
        begin    select @iOK = -102    goto fail_tran    end

    insert into [Log].dbo.IPLOItemPeriodItem
    with (updlock)
    (
        LoginUID
    ,   ItemUID
    ,   BuyerLogin
    ,   ItemID
    ,   RegDate
    ,   DelDate
    ,   StartDate
    ,   EndDate
    ,   Period
    ,   Expired
    )
    select
        @iLoginUID_
    ,   ItemUID
    ,   BuyerLogin
    ,   ItemID
    ,   RegDate
    ,   current_timestamp
    ,   StartDate
    ,   EndDate
    ,   Factor
    ,   1
    from @Item
    where
        No = 3
    and @iActionType = 2

    if @@error <> 0
    begin    select @iOK = -103    goto fail_tran    end

    update a with (updlock)
        set a.StrongLevel = @iStrongLevel_
            from dbo.GStrongItem as a with (updlock)
                where ItemUID = @iStrongItemUID_
                and @iActionType = 1
    if @@error <> 0
        begin    select @iOK = -104    goto fail_tran    end

    insert into dbo.GStrongItem with (updlock)
            (
                ItemUID
,               StrongLevel
            )
        select  @iStrongItemUID_
,               @iStrongLevel_
            where @iActionType = 0
    if @@error <> 0
        begin    select @iOK = -105    goto fail_tran    end

    insert into dbo.GStrongItemLog
            (
                LoginUID
,               ItemUID
,               ItemID
,               StrongLevel1
,               StrongLevel2
,               ActionType
,               RegDate
            )
        select  @iLoginUID_
,               @iStrongItemUID_
,               @iStrongItemID_
,               @iStrongLevel
,               @iStrongLevel_
,               @iActionType
,               @dtDate
    if @@error <> 0
        begin    select @iOK = -106    goto fail_tran    end
commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:
select  @iOK
,       isnull((select Factor from @Item where No = 1), 0)


