CREATE PROCEDURE [dbo].[up_schedule_approve_guild]
AS
SET NOCOUNT ON
SET XACT_ABORT ON

--길드 승인
update guildinfo with (rowlock)
    set approval = 1
        where approval = 0 and numMembers >= 5

--운영자/개발자 길드
update dbo.guildinfo with (updlock)
    set Point = Point + 2
        where guildid in (3786, --운영자길드
                          3875, --개발자길드
                          9486) --넷마블길드


