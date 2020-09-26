CREATE PROCEDURE [dbo].[VCGA_virtual_cash_select]
	@iLoginUID_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20090108. microcat.

L"{ call dbo.VCGA_virtual_cash_select ( %d ) }"

{ call dbo.VCGA_virtual_cash_select ( @1 ) }
@1 ; @iLoginUID_ int

1 return ( @1 )
@1 ; VCPoint int ; 리턴값이 없으면 0으로 취급
*/

select
    VCPoint
from dbo.VCGAVirtualCash
where
    LoginUID = @iLoginUID_


