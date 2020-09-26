CREATE PROCEDURE [dbo].[eup_santa_insert]
	@iEventUID_ [int],
	@strEventName_ [nvarchar](20),
	@sdtStartDate_ [smalldatetime],
	@sdtEndDate_ [smalldatetime],
	@sdtLimitDate_ [smalldatetime] = NULL,
	@strComment1_ [nvarchar](100) = NULL,
	@iItemID1_ [int] = NULL,
	@strComment2_ [nvarchar](100) = NULL,
	@iItemID2_ [int] = NULL,
	@strComment3_ [nvarchar](100) = NULL,
	@iItemID3_ [int] = NULL,
	@strComment4_ [nvarchar](100) = NULL,
	@iItemID4_ [int] = NULL,
	@strItemList_ [nvarchar](4000) = NULL,
	@iOK [int] = 0
AS
/*microcat
작성일 : 2007년 6월 26일
사용 : 산타 할배를 만들자
*/
set nocount on;

if exists(select * from dbo.ESantaEventUID with (nolock) where EventUID = @iEventUID_)
    begin    select @iOK = -1    goto end_proc    end

if @sdtStartDate_ > @sdtEndDate_
    begin    select @iOK = -2    goto end_proc    end

if @sdtLimitDate_ is not null and @sdtStartDate_ < @sdtLimitDate_
    begin    select @iOK = -3    goto end_proc    end

declare @Santa table
(
    UserType tinyint not null primary key clustered
,   Comment nvarchar(100) not null
,   ItemID int not null
)
if @strComment1_ is not null and @iItemID1_ is not null
    begin
        insert into @Santa(UserType, Comment, ItemID) select 1, @strComment1_, @iItemID1_
    end
if @strComment2_ is not null and @iItemID2_ is not null
    begin
        insert into @Santa(UserType, Comment, ItemID) select 2, @strComment2_, @iItemID2_
    end
if @strComment3_ is not null and @iItemID3_ is not null
    begin
        insert into @Santa(UserType, Comment, ItemID) select 4, @strComment3_, @iItemID3_
    end
if @strComment4_ is not null and @iItemID4_ is not null
    begin
        insert into @Santa(UserType, Comment, ItemID) select 8, @strComment4_, @iItemID4_
    end

if not exists(select * from @Santa)
    begin    select @iOK = -4    goto end_proc    end

declare @strToken nvarchar(1)
select  @strToken = N','

while right(@strItemList_, 1) = @strToken
    begin
        select @strItemList_ = substring(@strItemList_, 1, len(@strItemList_) - 1)
    end

declare @iUserType tinyint
,       @iItemID int
,       @iFactor int
,       @iStart int
,       @iEnd int
select  @iStart = 1
,       @iEnd = 1

declare @ItemList table
(
    UserType tinyint not null
,   ItemID int not null
,   Factor int not null
)

while 1 = 1
    begin
        select @iEnd = charindex(@strToken, @strItemList_, @iStart + 1)
        if @iEnd = 0
            begin    select @iOK = -5    goto end_proc    end
        select @iUserType = substring(@strItemList_, @iStart, @iEnd - @iStart)
        select @iStart = @iEnd + 1

        select @iEnd = charindex(@strToken, @strItemList_, @iStart + 1)
        if @iEnd = 0
            begin    select @iOK = -6    goto end_proc    end
        select @iItemID = substring(@strItemList_, @iStart, @iEnd - @iStart)
        select @iStart = @iEnd + 1

        select @iEnd = charindex(@strToken, @strItemList_, @iStart + 1)
        if @iEnd = 0
            begin    select @iEnd = 4000    end
        select @iFactor = substring(@strItemList_, @iStart, @iEnd - @iStart)

        if not exists(select * from @Santa where UserType = @iUserType)
            begin    select @iOK = -7    goto end_proc    end

        insert into @ItemList
                (
                    UserType
,                   ItemID
,                   Factor
                )
            select  @iUserType
,                   @iItemID
,                   @iFactor

        if @iEnd = 4000
            begin    break    end

        select @iStart = @iEnd + 1
    end

if not exists(select * from @ItemList)
    begin    select @iOK = -8    goto end_proc    end

if exists(select * from @Santa as a where not exists(select * from @ItemList as b where a.UserType = b.UserType))
    begin    select @iOK = -9    goto end_proc    end

begin tran
    insert into dbo.ESantaEventUID
            (
                EventUID
,               Name
            )
        select  @iEventUID_
,               @strEventName_
    if @@error <> 0
        begin    select @iOK = -11    goto fail_tran    end

    insert into dbo.ESanta
            (
                EventUID
,               UserType
,               Comment
,               ItemID
            )
        select  @iEventUID_
,               UserType
,               Comment
,               ItemID
            from @Santa
    if @@error <> 0
        begin    select @iOK = -12    goto fail_tran    end

    insert into dbo.ESantaDate
            (
                EventUID
,               StartDate
,               EndDate
            )
        select  @iEventUID_
,               @sdtStartDate_
,               @sdtEndDate_
    if @@error <> 0
        begin    select @iOK = -13    goto fail_tran    end

    insert into dbo.ESantaLimitDate
            (
                EventUID
,               LimitDate
            )
        select  top 1
                @iEventUID_
,               @sdtLimitDate_
            from @Santa
                where UserType in (2, 4)
    if @@error <> 0
        begin    select @iOK = -14    goto fail_tran    end

    insert into dbo.ESantaItem
            (
                EventUID
,               UserType
,               ItemID
,               Factor
            )
        select  @iEventUID_
,               UserType
,               ItemID
,               Factor
            from @ItemList
    if @@error <> 0
        begin    select @iOK = -15    goto fail_tran    end
commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:
select @iOK as OK


