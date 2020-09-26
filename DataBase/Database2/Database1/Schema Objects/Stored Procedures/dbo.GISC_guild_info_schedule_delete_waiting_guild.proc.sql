CREATE PROCEDURE [dbo].[GISC_guild_info_schedule_delete_waiting_guild]
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081224. microcat.
*/

declare
    @sdtDateD datetime
select
    @sdtDateD = convert(nvarchar(10), current_timestamp, 120)

declare
    @Data table
(
    No int not null identity(1, 1)
,   GuildID int not null
,   CafeURL nvarchar(20) collate database_default not null
)
insert into @Data
(
    GuildID
,   CafeURL
)
select
    GuildId
,   cafe_url
from dbo.GuildInfo
where
    Approval = 0
and RegDate <= dateadd(dd, -14, @sdtDateD)

declare
    @iLoop int
,   @iLoopMax int
,   @iGuildID int
,   @strCafeURL nvarchar(20)
select
    @iLoop = 1
,   @iLoopMax = max(No)
from @Data

while @iLoop <= @iLoopMax
begin
    select
        @iGuildID = GuildID
    ,   @strCafeURL = CafeURL
    from @Data
    where
        No = @iLoop

exec dbo.sp_executesql
    N'
execute [newclubdb].nmclub3.dbo.USP_N7GuildClubClose @1, @2, NULL, NULL, NULL, NULL, @2;
DELETE FROM dbo.GuildUserState WHERE GuildId = @3;
DELETE FROM dbo.Guildinfo WHERE GuildId = @3;
DELETE FROM dbo.GuildUser WHERE GuildId = @3;
DELETE FROM dbo.Notice  WHERE GuildId = @3;
'
,   N'@1 nvarchar(20), @2 nvarchar(5), @3 int'
,   @strCafeURL
,   'B0705'
,   @iGuildID

    select
        @iLoop = @iLoop + 1
end


