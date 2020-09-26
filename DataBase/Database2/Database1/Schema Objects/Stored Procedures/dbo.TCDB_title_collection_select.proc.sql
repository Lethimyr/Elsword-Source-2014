CREATe PROCEDURE [dbo].[TCDB_title_collection_select]
AS
set nocount on
set transaction isolation level read uncommitted

/*
20090210. mk8253.
등록된 칭호 아이템 정보
*/

SELECT MainMissionID as Title
FROM dbo.TCGTitleCollection
WITH (nolock)
OPTION (MAXDOP 1)


