CREATE PROCEDURE [dbo].[GNGA_game_notice_select_Message]
	@iNo_ [int]
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081010. microcat.
*/

select
    Message
,   IntervalM
from dbo.GNGAGameNotice
where
    No = @iNo_
option (maxdop 1)


