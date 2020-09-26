CREATE PROCEDURE [dbo].[TCG_title_collection_accomplish]
	@iLoginUID_ [int],
	@iMainMissionID_ [int],
	@iOK [int] = 0
AS
set nocount on

if not exists
(
    select *
    from dbo.Users
    with (nolock)
    where
        LoginUID = @iLoginUID_
)
begin    select @iOK = -1    goto end_proc    end

if not exists
(
    select *
    from dbo.TCGTitleCollectionMain
    with (nolock)
    where
        LoginUID = @iLoginUID_
    and MainMissionID = @iMainMissionID_
    and Status = 1
)
begin    select @iOK = -2    goto end_proc    end

declare
    @sdtDate smalldatetime
select
    @sdtDate = dateadd(mi, 1, getdate())

begin tran
    update a
    with (updlock)
    set
        a.EndDate = @sdtDate
    from dbo.TCGTitleCollectionMain
    as a
    with (updlock)
    where
        LoginUID = @iLoginUID_
    and MainMissionID = @iMainMissionID_
    and Status = 1
    option (maxdop 1)
    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end
commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:
select
    @iOK


