CREATE PROCEDURE [dbo].[ISGA_inventory_size_update]
	@iLoginUID_ [int],
	@iExtendSize_ [smallint],
	@iOK [int] = 0,
	@iTotalSize [smallint] = 260
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081008. microcat.
*/

execute dbo.AAA_login_check
    @iLoginUID_
,   @iOK output

if @iOK <> 0
begin    select @iOK = -1    goto end_proc    end

if not exists
(
    select *
    from dbo.GInventoryInfo
    where
        LoginUID = @iLoginUID_
)
begin
    begin transaction

    insert into dbo.GInventoryInfo
    with (updlock)
    (
        LoginUID
    ,   Size
    )
    select
        @iLoginUID_
    ,   @iTotalSize
    option (maxdop 1)

    if @@error <> 0
    begin    select @iOK = -101    goto fail_tran    end

    commit transaction
end

begin transaction

update a
with (updlock)
set
    @iTotalSize = a.Size = a.Size + @iExtendSize_
from dbo.GInventoryInfo
as a
with (updlock)
where
    LoginUID = @iLoginUID_
option (maxdop 1)

if @@error <> 0
begin    select @iOK = -102    goto fail_tran    end

insert into dbo.GInventoryInfoLog
with (updlock)
(
    LoginUID
,   ExtendSize
)
select
    @iLoginUID_
,   @iExtendSize_
option (maxdop 1)

if @@error <> 0
begin    select @iOK = -103    goto fail_tran    end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
select
    @iOK
,   @iTotalSize


