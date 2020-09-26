CREATE PROCEDURE [dbo].[up_query_duration_item_list]
	@strLogin_ [nvarchar](20)
AS
set nocount on;

select
    DurationItemUID
,   GoodsID
,   Duration
from dbo.DurationItemObjectList
with (nolock)
where OwnerLogin = @strLogin_


