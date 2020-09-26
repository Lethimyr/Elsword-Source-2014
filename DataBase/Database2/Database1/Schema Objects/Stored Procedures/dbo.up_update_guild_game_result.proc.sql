CREATE PROCEDURE [dbo].[up_update_guild_game_result]
	@iGuildID_ [int],
	@iGuildPoint_ [int],
	@iOK [int] = 0
AS
exec dbo.up_log_set_proc_count 'up_update_guild_game_result'

SET NOCOUNT ON

declare @iTotalGuildPoint int
if @iGuildPoint_ < 0
    begin    goto fail_proc    end

select  @iTotalGuildPoint = Point
    from dbo.GuildInfo with (nolock)
        where GuildID = @iGuildID_
if @iTotalGuildPoint is null
    begin    goto fail_proc    end

begin tran
    update a
        set Point = @iTotalGuildPoint + @iGuildPoint_
            from dbo.GuildInfo as a with (nolock)
                where GuildID = @iGuildID_
    if @@error <> 0
        begin    goto fail_tran    end

commit tran

goto end_proc

fail_tran:
rollback tran

fail_proc:
select  @iOK = -1
,       @iTotalGuildPoint = 0

end_proc:
select @iOK, @iTotalGuildPoint


