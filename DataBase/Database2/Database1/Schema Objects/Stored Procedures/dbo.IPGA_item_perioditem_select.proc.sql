CREATE PROCEDURE [dbo].[IPGA_item_perioditem_select]
	@iLoginUID_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20090120. microcat.

L"{ call dbo.IPGA_item_perioditem_select ( %d ) }"

{ call dbo.IPGA_item_perioditem_select ( @1 ) }
@1 ; @iLoginUID_ [int],

n return ( @1 )
@1 ; ItemUID [int]
*/

declare
    @strLogin nvarchar(20)
select
    @strLogin = Login
from dbo.Users
where
    LoginUID = @iLoginUID_

declare
    @dtDateA datetime
select
    @dtDateA = current_timestamp

select
    ItemUID
from dbo.GoodsObjectList
where
    OwnerLogin = @strLogin
and RegDate <= StartDate
and EndDate < @dtDateA
and Period > 0


