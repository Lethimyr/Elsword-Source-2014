CREATE PROCEDURE [dbo].[TCG_title_collection_select_main]
	@iLoginUID_ [int],
	@iOK [int] = 0
AS
set nocount on

declare
    @sdtDate smalldatetime
select
    @sdtDate = getdate()

if not exists
(
    select *
    from dbo.TCGTitleCollection
    as a
    with (updlock)
    left outer join
    (
        select
            MainMissionID
        from dbo.TCGTitleCollectionMain
        with (updlock)
        where
            LoginUID = @iLoginUID_
    )
    as b
    on
        a.MainMissionID = b.MainMissionID
    where
        b.MainMissionID is null
)
begin    goto end_proc    end

begin tran
    insert into dbo.TCGTitleCollectionMain
    with (updlock)
    (
        LoginUID
    ,   MainMissionID
    ,   StartDate
    ,   EndDate
    )
    select
        @iLoginUID_
    ,   a.MainMissionID
    ,   @sdtDate
    ,   @sdtDate
    from dbo.TCGTitleCollection
    as a
    with (updlock)
    left outer join
    (
        select
            MainMissionID
        from dbo.TCGTitleCollectionMain
        with (updlock)
        where
            LoginUID = @iLoginUID_
    )
    as b
    on
        a.MainMissionID = b.MainMissionID
    where
        b.MainMissionID is null
    option (maxdop 1)
    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end
commit tran

goto end_proc

fail_tran:
rollback tran

end_proc:
select
    MainMissionID
,   Status
from dbo.TCGTitleCollectionMain
with (nolock)
where
    LoginUID = @iLoginUID_


