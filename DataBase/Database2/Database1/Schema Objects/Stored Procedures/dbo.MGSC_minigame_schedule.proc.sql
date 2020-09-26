create PROCEDURE dbo.MGSC_minigame_schedule
AS
set nocount on
set transaction isolation level read uncommitted

begin transaction

    INSERT INTO dbo.WIUser
    (
	    LoginUID,
	    SupplierUID,
	    IssueUID,
	    RegDate,
	    SupDate
    )
    select top 5
        loginuid
    ,   1060
    ,   1060
    ,   current_timestamp
    ,   current_timestamp
    from dbo.MGGMiniGame
    where gamemode = 0
    order by point desc
    
    if @@error <> 0
    begin goto fail_tran end

    INSERT INTO dbo.WIUser
    (
	    LoginUID,
	    SupplierUID,
	    IssueUID,
	    RegDate,
	    SupDate
    )
    select top 5
        loginuid
    ,   1060
    ,   1060
    ,   current_timestamp
    ,   current_timestamp
    from dbo.MGGMiniGame
    where gamemode = 1
    order by point desc
    
    if @@error <> 0
    begin goto fail_tran end

commit transaction

goto succeed_tran

fail_tran:
    rollback transaction

succeed_tran:
    truncate table dbo.MGGMiniGame

end_proc:


