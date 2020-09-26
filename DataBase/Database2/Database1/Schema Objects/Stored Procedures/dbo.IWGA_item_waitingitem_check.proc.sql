CREATE PROCEDURE [dbo].[IWGA_item_waitingitem_check]
	@iLoginUID_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081220. microcat.

L"{ call dbo.IWGA_item_waitingitem_check ( %d ) }"

{ call dbo.IWGA_item_waitingitem_check ( @1 ) }
@1 ; LoginUID int

1 return ( @1 )
@1 ; OK int
    0  ; 아이템을 받아야 됨
    -1 ; 유저 정보가 없음
    -2 ; 대기 아이템이 없음
*/

if not exists
(
    select *
    from dbo.Users
    where
        LoginUID = @iLoginUID_
)
begin    select @iOK = -1    goto end_proc    end

if not exists
(
    select *
    from dbo.WIUser
    where
        LoginUID = @iLoginUID_
    and Supplied = 0
)
begin    select @iOK = -2    goto end_proc    end

end_proc:
select
    @iOK


