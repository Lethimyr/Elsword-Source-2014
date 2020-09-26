CREATE PROCEDURE [dbo].[gms_mission_system_sub_get]
	@iLoginUID_ [int]
AS
/*microcat
작성일 : 2007년 10월 23일
사용 : 유저의 서브 미션 정보를 출력
*/
set nocount on;

--dbo.GMSMissionSystemLog의 Status
--1 : 기간 만료
--2 : 유저에 의해서 삭제됨
--3 : 유저에 의해서 미션 완료

select  MainMissionID
,       SubMissionID
,       ProgressRate
    from dbo.GMSMissionSystemSub with (nolock)
        where LoginUID = @iLoginUID_


