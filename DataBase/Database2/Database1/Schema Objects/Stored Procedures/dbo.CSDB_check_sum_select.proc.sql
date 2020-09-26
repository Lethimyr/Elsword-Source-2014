CREATE PROCEDURE [dbo].[CSDB_check_sum_select]
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20090105. microcat.
*/

select
    Type
,   Protocol
,   convert(bigint, [CheckSum]) as [CheckSum]
,   convert(nvarchar(23), RegDateA, 121) as RegDate
from dbo.CSGACheckSum


