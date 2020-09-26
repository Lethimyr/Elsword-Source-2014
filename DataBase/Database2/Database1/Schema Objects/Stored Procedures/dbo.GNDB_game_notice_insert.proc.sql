CREATE PROCEDURE [dbo].[GNDB_game_notice_insert]
	@iNo_ [int],
    @sdtStartDateB_ [smalldatetime],
    @sdtEndtDateB_ [smalldatetime],
	@strMessage_ [nvarchar](256),
	@iIntervalM_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081010. microcat.
*/

if exists
(
    select *
    from dbo.GNGAGameNotice
    where
        No = @iNo_
)
begin    select @iOK = -1    goto end_proc    end

if @iIntervalM_ < 1
begin    select @iOK = -2    goto end_proc    end

begin transaction

insert into dbo.GNGAGameNotice
with (updlock)
(
    No
,   StartDateB
,   EndDateB
,   Message
,   IntervalM
)
select
    @iNo_
,   @sdtStartDateB_
,   @sdtEndtDateB_
,   @strMessage_
,   @iIntervalM_
option (maxdop 1)

if @@error <> 0
begin    select @iOK = -101    goto fail_tran    end

commit transaction

goto end_proc

fail_tran:
rollback transaction

end_proc:
select
    @iOK as OK


