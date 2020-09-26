CREATE PROCEDURE [dbo].[BCW_book_coupon_insert]
	@strLogin_ [nvarchar](20),
	@iOK [int] = 0 OUTPUT
AS
set nocount on
set transaction isolation level read uncommitted

/*
20080902. microcat.
*/

declare
    @iLoginUID int
,   @iIssueUID int
,   @sdtDateB smalldatetime
select
    @iLoginUID = LoginUID
,   @iIssueUID = 1050
,   @sdtDateB = getdate()
from dbo.Users
where
    Login = @strLogin_

if @iLoginUID is null
begin    select @iOK = -1    goto end_proc    end

if not exists
(
    select *
    from dbo.WIIssue
    where
        IssueUID = @iIssueUID
)
begin    select @iOK = -2    goto end_proc    end


if not exists
(
    select *
    from dbo.WIItem
    where
        IssueUID = @iIssueUID
)
begin    select @iOK = -3    goto end_proc    end

if not exists
(
    select *
    from dbo.WISupplier
    where
        SupplierUID = @iIssueUID
)
begin    select @iOK = -4    goto end_proc    end

begin tran
    insert into dbo.WIUser
    with (updlock)
    (
        LoginUID
    ,   SupplierUID
    ,   IssueUID
    ,   RegDate
    ,   SupDate
    )
    select
        @iLoginUID
    ,   @iIssueUID
    ,   @iIssueUID
    ,   @sdtDateB
    ,   @sdtDateB
    option (maxdop 1)
    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end
commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:


