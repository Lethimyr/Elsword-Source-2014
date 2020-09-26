CREATE PROCEDURE [dbo].[SDGA_server_data_select_integer_user]
	@iLoginUID_ [int],
	@sdtRegDateD_ [smalldatetime],
	@iType_ [tinyint],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20090116. microcat.

L"{ call dbo.SDGA_server_data_select_integer_user ( %d, N'%s', %d ) }"

{ call dbo.SDGA_server_data_select_integer_user ( @1, @2, @3 ) }
@1 ; @iLoginUID_ [int]
@2 ; @sdtRegDateD_ [smalldatetime]  ; 일단위까지만 인식함
@3 ; @iType_ [tinyint]              ; 서버에서 원하는 숫자별로 분류됨

1 return ( @1 )
@1 ; Data int
*/

select
    Data
from dbo.SDGAServerDataIntegerUser
where
    LoginUID = @iLoginUID_
and RegDateD = convert(nvarchar(10), @sdtRegDateD_, 120)
and Type = @iType_


