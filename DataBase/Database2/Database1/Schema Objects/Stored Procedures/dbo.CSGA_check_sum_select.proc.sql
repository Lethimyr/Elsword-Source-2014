CREATE PROCEDURE [dbo].[CSGA_check_sum_select]
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20090105. microcat.

L"{ call dbo.CSGA_check_sum_select }"

{ call dbo.CSGA_check_sum_select }

n return ( @1, @2, @3 )
@1 ; Type tinyint
@2 ; Protocol int
@3 ; [CheckSum] binary(4)
*/

select
    Type
,   Protocol
,   [CheckSum]
from dbo.CSGACheckSum


