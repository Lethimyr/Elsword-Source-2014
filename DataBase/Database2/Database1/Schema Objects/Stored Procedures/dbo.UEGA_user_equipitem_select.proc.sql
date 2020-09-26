CREATE PROCEDURE [dbo].[UEGA_user_equipitem_select]
	@iLoginUID_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081127. microcat.

L"{ call dbo.UEGA_user_equipitem_select ( %d ) }"

{ call dbo.UEGA_user_equipitem_select ( @1 ) }
@1 ; LoginUID int

n return ( @1, @2, @3, @4 )
@1 ; ItemOrderNo tinyint
@2 ; ItemUID int
@3 ; ItemID int
@4 ; ItemType bit
*/

select
    ItemOrderNo
,   ItemUID
,   ItemID
,   ItemType
from dbo.UEGAUserEquipItem
where
    LoginUID = @iLoginUID_
and Deleted = 0


