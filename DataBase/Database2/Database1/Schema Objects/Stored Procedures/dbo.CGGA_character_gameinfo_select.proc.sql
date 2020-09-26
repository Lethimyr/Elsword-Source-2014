CREATE PROCEDURE [dbo].[CGGA_character_gameinfo_select]
	@iLoginUID_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081229. microcat.

L"{ call dbo.CGGA_character_gameinfo_select ( %d ) }"

{ call dbo.CGGA_character_gameinfo_select ( @1 ) }
@1 ; @iLoginUID_ int

n return ( @1, @2, @3, @4, @5 )
@1 ; CharType tinyint
    0 ; 엘리시스
    1 ; 리르
    2 ; 아르메
    3 ; 라스
    4 ; 라이언
    5 ; 로난
    6 ; 에이미
    7 ; 진
@2 ; Promotion tinyint
    0 ; 무전직
    1 ; 1차 전직
    2 ; 2차 전직
    3 ; 3차 전직
@3 ; [Exp] bigint
@4 ; Win int
@5 ; Lose int
*/

declare
    @strLogin nvarchar(20)
select
    @strLogin = Login
from dbo.Users
where
    LoginUID = @iLoginUID_

select
    CharType
,   Promotion
,   [Exp]
,   Win
,   Lose
from dbo.Characters
where
    Login = @strLogin


