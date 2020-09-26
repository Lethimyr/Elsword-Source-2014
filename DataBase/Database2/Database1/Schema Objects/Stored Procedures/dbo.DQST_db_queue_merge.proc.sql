CREATE PROCEDURE [dbo].[DQST_db_queue_merge]
	@strServerIP_ [nvarchar](16),
	@iServerPort_ [int],
	@iQueueSize_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081226. mk8253.

{ call dbo.DQST_db_queue_merge ( %s, %d, %d ) }

{ call dbo.DQST_db_queue_merge ( @1, @2, @3 ) }
@1 ; ServerIP int
@2 ; ServerPort int
@3 ; QueueSize int

1 return ( @1 )
@1 ; OK int

OK
0 ; 성공
-1 ; 유저가 존재하지 않음
-101이하 ; 트랜잭션 에러
*/

if not exists
(
    SELECT *
    FROM dbo.ConnectStatusDB
    WHERE
        ServerIP = @strServerIP_
    and ServerPort = @iServerPort_
)
begin select @iOK = -1 goto end_proc end

declare
    @iServerIP bigint

select
    @iServerIP = dbo.IPn_IPs(@strServerIP_)
    
if exists
(
    SELECT *
	FROM dbo.DQSTDbQueue
	WHERE
	    ServerIP = @iServerIP
    and ServerPort = @iServerPort_
)
begin
    begin transaction
        update a
        with (updlock)
        set a.QueueSize = @iQueueSize_
        FROM dbo.DQSTDbQueue as a
        WITH (updlock)
        WHERE
            ServerIP = @iServerIP
        and ServerPort = @iServerPort_
        
        if @@error <> 0
        begin select @iOK = -101 goto fail_tran end
end
else
begin
    begin transaction
        INSERT INTO dbo.DQSTDbQueue
        with (updlock)
        (
			ServerIP,
			ServerPort,
			QueueSize
		)
		select
		    @iServerIP
        ,   @iServerPort_
        ,   @iQueueSize_
        
        if @@error <> 0
        begin select @iOK = -102 goto fail_tran end
end

commit transaction

goto end_proc

fail_tran:
    rollback transaction

end_proc:
    select @iOK


