CREATE PROCEDURE [dbo].[TCG_title_collection_select_sub]
	@iLoginUID_ [int],
	@iOK [int] = 0
AS
set nocount on

select
    MainMissionID
,   SubMissionID
,   TCCount
from dbo.TCGTitleCollectionSub
with (nolock)
where
    LoginUID = @iLoginUID_


