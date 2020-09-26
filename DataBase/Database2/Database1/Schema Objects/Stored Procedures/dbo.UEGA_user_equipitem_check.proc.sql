CREATE PROCEDURE [dbo].[UEGA_user_equipitem_check]
	@iLoginUID_ [int],
	@iItemUID_ [int],
	@iItemID_ [int],
	@bItemType_ [bit],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081127. microcat.

L"{ call dbo.UEGA_user_equipitem_check ( %d, %d, %d, %d ) }"

{ call dbo.UEGA_user_equipitem_check ( @1, @2, @3, @4 ) }
@1 ; LoginUID int
@2 ; ItemUID int
@3 ; ItemID int
@4 ; ItemType bit

1 return ( @1 )
@1 ; OK int
    0  ; 성공
    -1 ; 유저가 존재하지 않음
    -2 ; 존재하지 않은 아이템을 장착 기록하려 함
    -3 ; 존재하지 않은 아이템을 장착 기록하려 함
*/

declare
    @strLogin nvarchar(20)
select
    @strLogin = Login
from dbo.Users
where
    LoginUID = @iLoginUID_

if @strLogin is null
begin    select @iOK = -1    goto end_proc    end

if @bItemType_ = 0
and not exists
(
    select *
    from dbo.GoodsObjectList
    where
        ItemUID = @iItemUID_
    and OwnerLogin = @strLogin
    and ItemID = @iItemID_
)
begin    select @iOK = -2    goto end_proc    end
else if @bItemType_ = 1
and not exists
(
    select *
    from dbo.DurationItemObjectList
    where
        DurationItemUID = @iItemUID_
    and OwnerLogin = @strLogin
    and GoodsID = @iItemID_
)
begin    select @iOK = -3    goto end_proc    end

end_proc:
select
    @iOK


