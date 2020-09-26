/*	sutamtwo
	OK값

	-1 : 삭제 에러
	-2 : 삭제 기록 작성 에러
*/

CREATE PROCEDURE [dbo].[gup_all_dungeonclear]
	@iUnitUID_		[bigint]

AS
SET NOCOUNT ON;

		INSERT INTO dbo.GDungeonClear (UnitUID, GameMode, MaxScore,MaxTotalRank,regdate)
			SELECT @iUnitUID_, 	30000	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30001	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30002	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30010	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30011	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30012	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30020	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30021	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30022	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30030	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30031	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30032	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30040	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30041	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30042	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30050	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30051	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30052	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30060	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30061	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30062	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30070	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30071	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30072	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30080	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30081	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30082	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30090	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30091	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30092	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30100	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30101	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30102	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30110	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30111	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30112	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30120	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30121	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30122	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30130	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30131	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30132	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30140	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30141	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30142	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30150	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30151	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30152	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30160	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30161	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30162	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30170	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30171	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30172	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30180	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30181	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30182	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30190	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30191	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30192	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30200	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30201	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30202	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30210	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30211	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30212	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30220	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30221	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30222	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30230	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30231	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30232	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30240	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30241	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30242	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30250	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30251	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30252	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30260	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30261	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30262	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30270	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30271	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30272	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30280	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30281	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30282	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30290	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30291	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30292	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30300	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30301	, 0, 0, getdate() union all
			SELECT @iUnitUID_, 	30302	, 0, 0, getdate()


