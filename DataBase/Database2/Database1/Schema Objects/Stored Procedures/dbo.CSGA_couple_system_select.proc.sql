CREATE PROCEDURE [dbo].[CSGA_couple_system_select]
	@iLoginUID_ [int],
	@iOK [int] = 0
AS
set nocount on
set transaction isolation level read uncommitted

/*
20081027. microcat.

{ call dbo.CSGA_couple_system_select ( %d ) }

{ call dbo.CSGA_couple_system_select ( @1 ) }
@1 ; LoginUID int

1 return ( @1, @2, @3, @4 )
@1 ; LoginUID int ; 내꺼
@2 ; CharType tinyint ; 내꺼 선택된 캐릭터
@3 ; RegDateA datetime ; 내꺼 커플이 된 시각
@4 ; Nick nvarchar(24) ; 빈데이터임 패스
union all
1 return ( @1, @2, @3, @4 )
@1 ; LoginUIDE int ; 커플꺼
@2 ; CharTypeE tinyint ; 커플꺼 선택된 캐릭터
@3 ; RegDateA datetime ; 커플꺼 커플이 된 시각
@4 ; Nick nvarchar(24) ; 커플꺼 닉네임

첫번째줄과 두번째줄이 리턴되는 경우 ; 커플 상태임
첫번째줄만 리턴되는 경우 ; 상대편이 커플 해제한 상태임
리턴되는 것이 없는 경우 ; 커플 상태 아님
*/

declare
    @iLoginUID int
,   @iLoginUIDE int
,   @dtRegDateA datetime
select
    @iLoginUID = LoginUID
,   @iLoginUIDE = LoginUIDE
,   @dtRegDateA = RegDateA
from dbo.CSGACoupleSystem
where
    LoginUID = @iLoginUID_

declare
    @dtRegDateAE datetime
select
    @dtRegDateAE = RegDateA
from dbo.CSGACoupleSystem
where
    LoginUID = @iLoginUIDE
and LoginUIDE = @iLoginUID

declare
    @iCharType tinyint
select
    @iCharType = CharType
from dbo.CSGACoupleSystemCharacter
where
    LoginUID = @iLoginUID

declare
    @iCharTypeE tinyint
select
    @iCharTypeE = CharType
from dbo.CSGACoupleSystemCharacter
where
    LoginUID = @iLoginUIDE

declare
    @strLoginE nvarchar(20)
select
    @strLoginE = Login
from dbo.Users
where
    LoginUID = @iLoginUIDE

declare
    @strNickE nvarchar(24)
select
    @strNickE = Nick
from dbo.NickNames
where
    Login = @strLoginE

select
    @iLoginUID
,   @iCharType
,   @dtRegDateA
,   N'gg'
where
    @dtRegDateA is not null
union all
select
    @iLoginUIDE
,   @iCharTypeE
,   @dtRegDateAE
,   @strNickE
where
    @dtRegDateAE is not null


