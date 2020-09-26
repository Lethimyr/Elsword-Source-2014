CREATE PROCEDURE [dbo].[TCDB_title_collection_insert]
	@iMainTitle_ [int],
	@iSubTitle_ [int],
	@strTitleName_ [nvarchar](20),
	@strDungeon_ [nvarchar](20),
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20090210. mk8253.
등록된 칭호 아이템 정보
*/

if not exists
(
    SELECT *
    FROM dbo.GoodsInfoList
    WHERE GoodsID = @iMainTitle_
)
begin select @iOK = -1 goto end_proc end

if exists
(
    SELECT *
    FROM dbo.GMessageName
    WHERE ItemID = @iMainTitle_
)
begin select @iOK = -2 goto end_proc end

if exists
(
    SELECT *
    FROM dbo.TCGTitleCollection
    WHERE MainMissionID in
    (
        @iMainTitle_
    ,   @iSubTitle_
    )
)
begin select @iOK = -3 goto end_proc end

begin transaction
    INSERT INTO dbo.TCGTitleCollection
    with (updlock)
    (
		MainMissionID
	)
    select @iMainTitle_
    union all select @iSubTitle_
	
    if @@error <> 0
    begin select @iOK = -101 goto fail_tran end
    
    INSERT INTO dbo.GMessageName
    with (updlock)
    (
		ItemID
    ,   Offset
    ,	[Name]
    ,	MessageID
	)
	select
	    @iMainTitle_
    ,   0
    ,   @strTitleName_
    ,   4
    
    if @@error <> 0
    begin select @iOK = -102 goto fail_tran end
    
    INSERT INTO dbo.GMessageName
    with (updlock)
    (
		ItemID
    ,   Offset
    ,	[Name]
    ,	MessageID
	)
	select
	    @iMainTitle_
    ,   1
    ,   @strDungeon_
    ,   4
    
    if @@error <> 0
    begin select @iOK = -103 goto fail_tran end
commit transaction

goto end_proc

fail_tran:
    rollback transaction

end_proc:
    select @iOK as OK


