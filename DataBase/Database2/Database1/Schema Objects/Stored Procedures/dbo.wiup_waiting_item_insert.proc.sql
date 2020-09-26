CREATE PROCEDURE [dbo].[wiup_waiting_item_insert]
	@strLogins_ [nvarchar](4000),
	@strItems_ [nvarchar](4000),
	@strSupplier_ [nvarchar](20),
	@iOK [int] = 0,
	@iError [int] = 0
AS
/*microcat
작성일 : 2007년 8월 11일
사용 : 대기 아이템 입력
*/
set nocount on;

----
declare @strToken nvarchar(1)
select  @strToken = N','
while right(@strLogins_, 1) = @strToken
    begin
        select @strLogins_ = substring(@strLogins_, 1, len(@strLogins_) - 1)
    end

declare @iLoginUID int
,       @strLogin nvarchar(20)
,       @iStart int
,       @iEnd int
select  @iStart = 1
,       @iEnd = 1

declare @User table
(
    LoginUID int not null
,   Login nvarchar(20) not null
,   No int not null identity(1, 1)
,   primary key clustered(LoginUID, No)
)

while 1 = 1
    begin
        select @iEnd = charindex(@strToken, @strLogins_, @iStart + 1)
        if @iEnd = 0
            begin    select @iEnd = 4000    end
        select @strLogin = substring(@strLogins_, @iStart, @iEnd - @iStart)
        select @iLoginUID = LoginUID
            from dbo.Users with (nolock)
                where Login = @strLogin
        if @iLoginUID is null
            begin    select @iOK = -1    goto end_proc    end

        insert into @User
                (
                    LoginUID
,                   Login
                )
            select  @iLoginUID
,                   @strLogin

        if @iEnd = 4000
            begin    break    end

        select @iStart = @iEnd + 1, @iLoginUID = null
    end

if not exists(select * from @User)
    begin    select @iOK = -2    goto end_proc    end

----
while right(@strItems_, 1) = @strToken
    begin
        select @strItems_ = substring(@strItems_, 1, len(@strItems_) - 1)
    end

declare @iItemID int
,       @iFactor int
,       @iExhaustionType tinyint
,       @iPropertyType tinyint
select  @iStart = 1
,       @iEnd = 1

declare @Item table
(
    ItemID int not null
,   Factor int not null
,   ExhaustionType tinyint not null
,   PropertyType tinyint not null
,   No int not null identity(1, 1)
,   primary key clustered(ItemID, No)
)

while 1 = 1
    begin
        select @iEnd = charindex(@strToken, @strItems_, @iStart + 1)
        if @iEnd = 0
            begin    select @iOK = -3    goto end_proc    end
        select @iItemID = substring(@strItems_, @iStart, @iEnd - @iStart)
        select @iStart = @iEnd + 1

        select @iEnd = charindex(@strToken, @strItems_, @iStart + 1)
        if @iEnd = 0
            begin    select @iEnd = 4000    end
        select @iFactor = substring(@strItems_, @iStart, @iEnd - @iStart)

        select  @iExhaustionType = case when ItemType = 0 and @iFactor > 0 then 2 else ItemType end
,               @iPropertytype = IsSubSetGoods
            from dbo.GoodsInfoList with (nolock)
                where GoodsID = @iItemID
        if @iExhaustionType is null
            begin    select @iOK = -4    goto end_proc    end

        if exists(select * from dbo.CashPackageInfoList with (nolock) where ItemID = @iItemID)
            or exists(select * from dbo.PackageInfoList with (nolock) where ItemID = @iItemID)
            begin    select @iPropertyType = 2    end

        if (@iExhaustionType = 0 and @iFactor <> -1) or (@iExhaustionType between 1 and 2 and @iFactor <= 0)
            begin    select @iOK = -5    goto end_proc    end

        insert into @Item
                (
                    ItemID
,                   Factor
,                   ExhaustionType
,                   PropertyType
                )
            select  @iItemID
,                   @iFactor
,                   @iExhaustionType
,                   @iPropertyType

        if @iEnd = 4000
            begin    break    end

        select @iStart = @iEnd + 1, @iExhaustionType = null
    end

if not exists(select * from @Item)
    begin    select @iOK = -6    goto end_proc    end

----
declare @iSupplierUID int
,       @iIssueUID int
,       @sdtDate smalldatetime
select  @iSupplierUID = SupplierUID
    from dbo.WISupplier with (nolock)
        where Name = @strSupplier_
select  @sdtDate = getdate()

----
begin tran
    if @iSupplierUID is null
        begin
            insert into dbo.WISupplier with (updlock)
                    (
                        Name
                    )
                select @strSupplier_
            if @@error <> 0
                begin    select @iOK = -101, @iError = @@error    goto fail_tran    end
            select @iSupplierUID = @@identity
        end
    
    insert into dbo.WIIssue with (updlock)
        default values
    if @@error <> 0
        begin    select @iOK = -102, @iError = @@error    goto fail_tran    end
    select @iIssueUID = @@identity

    insert into dbo.WIUser with (updlock)
            (
                LoginUID
,               SupplierUID
,               IssueUID
,               RegDate
,               SupDate
            )
        select  LoginUID
,               @iSupplierUID
,               @iIssueUID
,               @sdtDate
,               @sdtDate
            from @User
    if @@error <> 0
        begin    select @iOK = -103, @iError = @@error    goto fail_tran    end

    insert into dbo.WIItem with (updlock)
            (
                IssueUID
,               ItemID
,               Factor
,               ExhaustionType
,               PropertyType
            )
        select  @iIssueUID
,               ItemID
,               Factor
,               ExhaustionType
,               PropertyType
            from @Item
commit tran

goto end_proc

fail_tran:
rollback tran

----
end_proc:
if @iOK <> 0
    begin
        declare @strDB nvarchar(4000)
,               @strProc nvarchar(4000)
        select  @strDB = db_name()
,               @strProc = object_name(@@procid)
        raiserror('microcat : %s.dbo.%s(OK : %d, Error : %d, Supplier : %s)', 16, 127, @strDB, @strProc, @iOK, @iError, @strSupplier_)
    end
select  convert(nvarchar(10), @iOK) as Login
,       @iOK as ItemID
,       @iOK as Factor
,       @iOK as ExhaustionType
,       @iOK as PropertyType
union all
select  a.Login
,       b.ItemID
,       b.Factor
,       b.ExhaustionType
,       b.PropertyType
    from @User as a
    cross join @Item as b


