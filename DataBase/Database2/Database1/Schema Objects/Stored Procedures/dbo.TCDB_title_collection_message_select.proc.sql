CREATE PROCEDURE [dbo].[TCDB_title_collection_message_select]
AS
set nocount on
set transaction isolation level read uncommitted

/*
20090210. mk8253.
등록된 칭호 아이템 정보
*/

SELECT a.ItemID, a.Name as TitleCollection, b.Name as Dungeon
FROM dbo.GMessageName as a
WITH (nolock)
join dbo.GMessageName as b
WITH (nolock)
on a.ItemID = b.ItemID
and a.Offset = 0
and b.Offset = 1
where a.MessageID = 4
OPTION (maxdop 1)


