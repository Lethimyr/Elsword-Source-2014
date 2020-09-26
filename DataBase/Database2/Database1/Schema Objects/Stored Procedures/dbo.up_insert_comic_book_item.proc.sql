CREATE PROCEDURE [dbo].[up_insert_comic_book_item]
	@strLogin_ [nvarchar](20),
	@strSN_ [nvarchar](50),
	@iCharType_ [tinyint] = 255,
	@iOK [int] = 0
AS
SET NOCOUNT ON;

DECLARE @iGoodsID int, @iDuration int
SELECT @iGoodsID = -1, @iDuration = -1

IF NOT EXISTS(SELECT * FROM dbo.Users(NOLOCK) WHERE Login = @strLogin_)
    BEGIN    SELECT @iOK = -1    GOTO END_PROC    END

DECLARE @strItemName nvarchar(50), @bUse bit
SELECT @strItemName = '', @bUse = 0
SELECT @strItemName = ItemName, @bUse = bUse FROM dbo.ComicBookItemInfo(NOLOCK) WHERE SerialNumber = @strSN_
IF @strItemName = ''
    BEGIN    SELECT @iOK = -2    GOTO END_PROC    END
IF @bUse = 1
    BEGIN    SELECT @iOK = -4    GOTO END_PROC    END

DECLARE @iMaxCharType tinyint
SELECT TOP 1 @iMaxCharType = CharType FROM dbo.Characters(NOLOCK) WHERE login = @strLogin_ ORDER BY [Exp] DESC

DECLARE @strSQL nvarchar(4000)
IF @strItemName <> '게임 포인트 5,000'
    BEGIN
        IF @iCharType_ = 255
            BEGIN
                SELECT @iGoodsID = CASE @strItemName
                            WHEN '수호의 반지(30회)'            THEN 12710
                            WHEN '그체 교복 기사 복장세트'      THEN 24770
                            WHEN '그체 교복 궁수 복장세트'      THEN 24780
                            WHEN '그체 교복 법사 복장세트'      THEN 24790
                            WHEN '그체 교복 도적 복장세트'      THEN 24800
                            WHEN 'Gem 15개'                     THEN 8880
                            WHEN '더블크리스탈 카드(30회)'      THEN 12500
                            WHEN '눈꽃 요정의 반지'             THEN 12700
                            WHEN '불꽃 레어 아이템'             THEN 16700
                            WHEN '신기술3단계(궁수) - 500회'    THEN 21620
                            WHEN '신기술3단계(기사) - 500회'    THEN 21600
                            WHEN '신기술3단계(법사) - 500회'    THEN 21650
                            WHEN '수호의 반지(10회)'            THEN 19360
                            WHEN '암흑 강화 목걸이 7단계'       THEN CASE @imaxCharType WHEN 3 THEN 20980 ELSE 15640 + 100 * @iMaxCharType END
                            WHEN '암흑 강화 목걸이 9단계'       THEN CASE @imaxCharType WHEN 3 THEN 21000 ELSE 15660 + 100 * @iMaxCharType END
                            WHEN '불꽃 강화 목걸이 9단계'       THEN CASE @imaxCharType WHEN 3 THEN 20740 ELSE 11930 + 100 * @iMaxCharType END
                            --코믹북8,9권추가시작
                            WHEN '공작 깃털 날개(100회)'        THEN 16690
                            WHEN '카제아제의 힐링볼(200회)'     THEN 17470
                            WHEN '용사의 반지'                  THEN 29920
                            WHEN '태양 머리장식'                THEN 30790
                            WHEN '태양 날개장식'                THEN 30800
                            WHEN '태양 팔장식'                  THEN 30810
                            --코믹북8,9권추가완료
                            --코믹북12,13,14권추가시작
                            WHEN '절대반지'                     THEN 40	 
                            WHEN '수호의 반지(60회)'            THEN 12710
                            WHEN 'GC 스타'                      THEN 30470
                            WHEN '크라운 스타'                  THEN 31730
                            WHEN '+60 가방'                     THEN 42220
                            WHEN '닌코 봉인카드'                THEN 42350
                            --코믹북12,13,14권추가완료
                            --코믹북 2.0 추가시작
                            when '코믹2.0 기사망토'             then 50490
                            when '코믹2.0 궁수망토'             then 50500
                            when '코믹2.0 법사망토'             then 50510
                            --코믹북 2.0 추가완료
                            --071004. microcat. 이랜드 제휴 수호의 반지 20회 - 30,000
                            when '이랜드 제휴 수호의 반지 20회' then 51300
                            --071004. microcat. 이랜드 제휴 수호의 반지 20회 - 30,000
                            --071025. microcat. 그랜드체이스 뮤지컬 이벤트 - 10,000
                            when '방랑자 로난 마검'             then 54820
                            when '방랑자 로난 복장세트'         then 54830
                            when '로난 획득 미션'               then 55180
                            --071025. microcat. 그랜드체이스 뮤지컬 이벤트 - 10,000
                            --071129. microcat. 가이드북2 - 5,100
                            when '가디언나이츠 기사 복장세트'       then 50590
                            when '가디언나이츠 궁수 복장세트'       then 50660
                            when '가디언나이츠 법사 복장세트'       then 50730
                            when '가디언나이츠 도적 복장세트'       then 50800
                            when '가디언나이츠 드루이드 복장세트'   then 50870
                            when '가디언나이츠 마검사 복장세트'     then 70010
                            --071129. microcat. 가이드북2 - 5,100
                            --080402. microcat. 코믹북 16권
                            when '080402 여왕의보석 42410 7일'  then 42410
                            when '080402 No1 31720 30일'        then 31720
                            when '080402 수호의반지 12710 60회' then 12710
                            when '080402 잼 8880 100개'         then 8880
                            when '080402 GC스타 30470 30일'     then 30470
                            when '080402 절대반지 40 30일'      then 40
                            --080402. microcat. 코믹북 16권
                            --080411. microcat. 뮤지컬
                            when '080411 카나반의증표 영구'     then 80410
                            when '080411 카나반이모티콘 영구'   then 80420
                            when '080411 카나반의날개 영구'     then 80430
                            when '080411 카나반의망또 영구'     then 79430
                            --080411. microcat. 뮤지컬
                            --080514. microcat. 가이드북
                            when '080514 카니발기사복장세트 180일'      then 74510
                            when '080514 카니발궁수복장세트 180일'      then 74520
                            when '080514 카니발마법사복장세트 180일'    then 74530
                            when '080514 카니발도적복장세트 180일'      then 74540
                            when '080514 카니발드루이드복장세트 180일'  then 74550
                            when '080514 카니발마검사복장세트 180일'    then 74650
                            --080514. microcat. 가이드북
                            --080721. mk8253. 코믹북 17권
                            when '080721 닌코 봉인카드 영구'    then 42350
                            when '080721 GC 스타 15일'          then 30470
                            when '080721 수호의 반지 60회'      then 12710
                            when '080721 수호의 반지 30회'      then 51300
                            when '080721 +30 가방 1회'          then 43500
                            when '080721 절대반지 15일'         then 40
                            --080721. mk8253. 코믹북 17권
                            --081027. mk8253. 코믹북 18권
                            when '081027 큐피트 봉인카드 영구'      then 79050
                            when '081027 GC 스타 30일'              then 30470
                            when '081027 명예의 금반지 30일'        then 89630
                            when '081027 강화율 상승 아이템 3회'    then 71770
                            when '081027 이지스 방패 30일'          then 88460
                            when '081027 절대 반지 30일'            then 40
                            --081027. mk8253. 코믹북 18권
                            --081110. mk8253. 코믹북 18권 추가
                            when '081110 큐피트 봉인카드 영구'      then 79050
                            when '081110 GC 스타 30일'              then 30470
                            when '081110 명예의 금반지 30일'        then 89630
                            when '081110 강화율 상승 아이템 3회'    then 71770
                            when '081110 이지스 방패 30일'          then 88460
                            when '081110 절대 반지 30일'            then 40
                            --081110. mk8253. 코믹북 18권 추가
                            --081110. mk8253. 코믹북 17권 추가
                            when '081110 닌코 봉인카드 영구'    then 42350
                            when '081110 GC 스타 15일'          then 30470
                            when '081110 수호의 반지 60회'      then 12710
                            when '081110 수호의 반지 30회'      then 51300
                            when '081110 +30 가방 1회'          then 43500
                            when '081110 절대반지 15일'         then 40
                            --081110. mk8253. 코믹북 17권 추가
                            --081111. mk8253. 뮤지컬
                            when '081111 GC클럽 30일'    then 42340
                            --081111. mk8253. 뮤지컬
                            --081112. mk8253. 코믹북 16권
                            when '081112 여왕의보석 42410 7일'  then 42410
                            when '081112 No1 31720 30일'        then 31720
                            when '081112 수호의반지 12710 60회' then 12710
                            when '081112 잼 8880 100개'         then 8880
                            when '081112 GC스타 30470 30일'     then 30470
                            when '081112 절대반지 40 30일'      then 40
                            --081112. mk8253. 코믹북 16권
                            --081112. mk8253. 코믹북 10권
                            when '081112 태양 머리 장식 30일'       then 30790
                            when '081112 태양 날개 장식 30일'       then 30800
                            when '081112 태양 팔 장식 30일'         then 30810
                            when '081112 용사의 반지 30일'          then 29920
                            when '081112 카제아제 힐링 볼 200회'    then 17470
                            when '081112 공작 깃털 날개 100회'      then 16690
                            --081112. mk8253. 코믹북 10권
                            --090115. mk8253. 오프라인대회
                            when '090115 오프라인대회' then 88460
                            --090115. mk8253. 오프라인대회

                            --20090119. microcat. 코믹북15권
                            when '20090119 닌코 봉인카드 42350 -1'     then 42350
                            when '20090119 GC 스타 (30일) 30470 30'    then 30470
                            when '20090119 크라운스타(30일) 31730 30'  then 31730
                            when '20090119 수호의 반지(60회) 12710 60' then 12710
                            when '20090119 +60 가방(1회) 42220 1'      then 42220
                            when '20090119 절대반지(30일) 40 30'       then 40
                            --20090119. microcat. 코믹북15권

                        END,
                       @iDuration = CASE @strItemName
                            WHEN '수호의 반지(30회)'            THEN 60
                            WHEN '그체 교복 기사 복장세트'      THEN 30
                            WHEN '그체 교복 궁수 복장세트'      THEN 30
                            WHEN '그체 교복 법사 복장세트'      THEN 30
                            WHEN '그체 교복 도적 복장세트'      THEN 30
                            WHEN 'Gem 15개'                     THEN 15
                            WHEN '더블크리스탈 카드(30회)'      THEN 30
                            WHEN '눈꽃 요정의 반지'             THEN -1
                            WHEN '불꽃 레어 아이템'             THEN 400
                            WHEN '신기술3단계(궁수) - 500회'    THEN 500
                            WHEN '신기술3단계(기사) - 500회'    THEN 500
                            WHEN '신기술3단계(법사) - 500회'    THEN 500
                            WHEN '수호의 반지(10회)'            THEN 10
                            WHEN '암흑 강화 목걸이 7단계'       THEN -1
                            WHEN '암흑 강화 목걸이 9단계'       THEN -1
                            WHEN '불꽃 강화 목걸이 9단계'       THEN -1
                            --코믹북8,9권추가시작
                            WHEN '공작 깃털 날개(100회)'        THEN 100
                            WHEN '카제아제의 힐링볼(200회)'     THEN 200
                            WHEN '용사의 반지'                  THEN -1
                            WHEN '태양 머리장식'                THEN -1
                            WHEN '태양 날개장식'                THEN -1
                            WHEN '태양 팔장식'                  THEN -1
                            --코믹북8,9권추가완료
                            --코믹북12,13,14권추가시작
                            WHEN '절대반지'                     THEN 30	 
                            WHEN '수호의 반지(60회)'            THEN 60
                            WHEN 'GC 스타'                      THEN 30
                            WHEN '크라운 스타'                  THEN 30
                            WHEN '+60 가방'                     THEN 1
                            WHEN '닌코 봉인카드'                THEN -1
                            --코믹북12,13,14권추가완료
                            --코믹북 2.0 추가시작
                            when '코믹2.0 기사망토'             then 30
                            when '코믹2.0 궁수망토'             then 30
                            when '코믹2.0 법사망토'             then 30
                            --코믹북 2.0 추가완료
                            --071004. microcat. 이랜드 제휴 수호의 반지 20회 - 30,000
                            when '이랜드 제휴 수호의 반지 20회' then 20
                            --071004. microcat. 이랜드 제휴 수호의 반지 20회 - 30,000
                            --071025. microcat. 그랜드체이스 뮤지컬 이벤트 - 10,000
                            when '방랑자 로난 마검'             then -1
                            when '방랑자 로난 복장세트'         then -1
                            when '로난 획득 미션'               then -1
                            --071025. microcat. 그랜드체이스 뮤지컬 이벤트 - 10,000
                            --071129. microcat. 가이드북2 - 5,100
                            when '가디언나이츠 기사 복장세트'       then 180
                            when '가디언나이츠 궁수 복장세트'       then 180
                            when '가디언나이츠 법사 복장세트'       then 180
                            when '가디언나이츠 도적 복장세트'       then 180
                            when '가디언나이츠 드루이드 복장세트'   then 180
                            when '가디언나이츠 마검사 복장세트'     then 180
                            --071129. microcat. 가이드북2 - 5,100
                            --080402. microcat. 코믹북 16권
                            when '080402 여왕의보석 42410 7일'  then 7
                            when '080402 No1 31720 30일'        then 30
                            when '080402 수호의반지 12710 60회' then 60
                            when '080402 잼 8880 100개'         then 100
                            when '080402 GC스타 30470 30일'     then 30
                            when '080402 절대반지 40 30일'      then 30
                            --080402. microcat. 코믹북 16권
                            --080411. microcat. 뮤지컬
                            when '080411 카나반의증표 영구'     then -1
                            when '080411 카나반이모티콘 영구'   then 1
                            when '080411 카나반의날개 영구'     then -1
                            when '080411 카나반의망또 영구'     then -1
                            --080411. microcat. 뮤지컬
                            --080514. microcat. 가이드북
                            when '080514 카니발기사복장세트 180일'      then 180
                            when '080514 카니발궁수복장세트 180일'      then 180
                            when '080514 카니발마법사복장세트 180일'    then 180
                            when '080514 카니발도적복장세트 180일'      then 180
                            when '080514 카니발드루이드복장세트 180일'  then 180
                            when '080514 카니발마검사복장세트 180일'    then 180
                            --080514. microcat. 가이드북
                            --080721. mk8253. 코믹북 17권
                            when '080721 닌코 봉인카드 영구'    then -1
                            when '080721 GC 스타 15일'          then 15
                            when '080721 수호의 반지 60회'      then 60
                            when '080721 수호의 반지 30회'      then 30
                            when '080721 +30 가방 1회'          then 1
                            when '080721 절대반지 15일'         then 15
                            --080721. mk8253. 코믹북 17권
                            --081027. mk8253. 코믹북 18권
                            when '081027 큐피트 봉인카드 영구'      then -1
                            when '081027 GC 스타 30일'              then 30
                            when '081027 명예의 금반지 30일'        then 30
                            when '081027 강화율 상승 아이템 3회'    then 3
                            when '081027 이지스 방패 30일'          then 30
                            when '081027 절대 반지 30일'            then 30
                            --081027. mk8253. 코믹북 18권
                            --081110. mk8253. 코믹북 18권 추가
                            when '081110 큐피트 봉인카드 영구'      then -1
                            when '081110 GC 스타 30일'              then 30
                            when '081110 명예의 금반지 30일'        then 30
                            when '081110 강화율 상승 아이템 3회'    then 3
                            when '081110 이지스 방패 30일'          then 30
                            when '081110 절대 반지 30일'            then 30
                            --081110. mk8253. 코믹북 18권 추가
                            --081110. mk8253. 코믹북 17권 추가
                            when '081110 닌코 봉인카드 영구'    then -1
                            when '081110 GC 스타 15일'          then 15
                            when '081110 수호의 반지 60회'      then 60
                            when '081110 수호의 반지 30회'      then 30
                            when '081110 +30 가방 1회'          then 1
                            when '081110 절대반지 15일'         then 15
                            --081110. mk8253. 코믹북 17권 추가
                            --081111. mk8253. 뮤지컬
                            when '081111 GC클럽 30일'    then 30
                            --081111. mk8253. 뮤지컬
                            --081112. mk8253. 코믹북 16권
                            when '081112 여왕의보석 42410 7일'  then 7
                            when '081112 No1 31720 30일'        then 30
                            when '081112 수호의반지 12710 60회' then 60
                            when '081112 잼 8880 100개'         then 100
                            when '081112 GC스타 30470 30일'     then 30
                            when '081112 절대반지 40 30일'      then 30
                            --081112. mk8253. 코믹북 16권
                            --081112. mk8253. 코믹북 10권
                            when '081112 태양 머리 장식 30일'       then 30
                            when '081112 태양 날개 장식 30일'       then 30
                            when '081112 태양 팔 장식 30일'         then 30
                            when '081112 용사의 반지 30일'          then 30
                            when '081112 카제아제 힐링 볼 200회'    then 200
                            when '081112 공작 깃털 날개 100회'      then 100
                            --081112. mk8253. 코믹북 10권
                            --090115. mk8253. 오프라인대회
                            when '090115 오프라인대회' then 30
                            --090115. mk8253. 오프라인대회

                            --20090119. microcat. 코믹북15권
                            when '20090119 닌코 봉인카드 42350 -1'     then -1
                            when '20090119 GC 스타 (30일) 30470 30'    then 30
                            when '20090119 크라운스타(30일) 31730 30'  then 30
                            when '20090119 수호의 반지(60회) 12710 60' then 60
                            when '20090119 +60 가방(1회) 42220 1'      then 1
                            when '20090119 절대반지(30일) 40 30'       then 30
                            --20090119. microcat. 코믹북15권

                        END
                SELECT @strSQL = CASE @strItemName
                            WHEN '수호의 반지(30회)'            THEN '12710,60'
                            WHEN '그체 교복 기사 복장세트'      THEN '23730,30,23740,30,23750,30,23760,30,23770,30'
                            WHEN '그체 교복 궁수 복장세트'      THEN '24560,30,24570,30,24580,30,24590,30,24600,30'
                            WHEN '그체 교복 법사 복장세트'      THEN '24630,30,24640,30,24650,30,24660,30,24670,30'
                            WHEN '그체 교복 도적 복장세트'      THEN '24700,30,24710,30,24720,30,24730,30,24740,30'
                            WHEN 'Gem 15개'                     THEN '8880,15'
                            WHEN '더블크리스탈 카드(30회)'      THEN '12500,30'
                            WHEN '눈꽃 요정의 반지'             THEN '12700,-1'
                            WHEN '불꽃 레어 아이템'             THEN '16700,400'
                            WHEN '신기술3단계(궁수) - 500회'    THEN '21620,500'
                            WHEN '신기술3단계(기사) - 500회'    THEN '21600,500'
                            WHEN '신기술3단계(법사) - 500회'    THEN '21650,500'
                            WHEN '수호의 반지(10회)'            THEN '19360,10'
                            WHEN '암흑 강화 목걸이 7단계'       THEN CAST(@iGoodsID as nvarchar(10)) + ',-1'
                            WHEN '암흑 강화 목걸이 9단계'       THEN CAST(@iGoodsID as nvarchar(10)) + ',-1'
                            WHEN '불꽃 강화 목걸이 9단계'       THEN CAST(@iGoodsID as nvarchar(10)) + ',-1'
                            --코믹북8,9권추가시작
                            WHEN '공작 깃털 날개(100회)'        THEN '16690,100'
                            WHEN '카제아제의 힐링볼(200회)'     THEN '17470,200'
                            WHEN '용사의 반지'                  THEN '29920,-1'
                            WHEN '태양 머리장식'                THEN '30790,-1'
                            WHEN '태양 날개장식'                THEN '30800,-1'
                            WHEN '태양 팔장식'                  THEN '30810,-1'
                            --코믹북8,9권추가완료
                            --코믹북12,13,14권추가시작
                            WHEN '절대반지'                     THEN '40,30'
                            WHEN '수호의 반지(60회)'            THEN '12710,60'
                            WHEN 'GC 스타'                      THEN '30470,30'
                            WHEN '크라운 스타'                  THEN '31730,30'
                            WHEN '+60 가방'                     THEN '42220,1'
                            WHEN '닌코 봉인카드'                THEN '42350,-1'
                            --코믹북12,13,14권추가완료
                            --코믹북 2.0 추가시작
                            when '코믹2.0 기사망토'             then '50490,30'
                            when '코믹2.0 궁수망토'             then '50500,30'
                            when '코믹2.0 법사망토'             then '50510,30'
                            --코믹북 2.0 추가완료
                            --071004. microcat. 이랜드 제휴 수호의 반지 20회 - 30,000
                            when '이랜드 제휴 수호의 반지 20회' then '51300,20'
                            --071004. microcat. 이랜드 제휴 수호의 반지 20회 - 30,000
                            --071025. microcat. 그랜드체이스 뮤지컬 이벤트 - 10,000
                            when '방랑자 로난 마검'             then '54820,-1'
                            when '방랑자 로난 복장세트'         then '54830,-1'
                            when '로난 획득 미션'               then '55180,-1'
                            --071025. microcat. 그랜드체이스 뮤지컬 이벤트 - 10,000
                            --071129. microcat. 가이드북2 - 5,100
                            when '가디언나이츠 기사 복장세트'       then '50590,180,70020,-1'
                            when '가디언나이츠 궁수 복장세트'       then '50660,180,70020,-1'
                            when '가디언나이츠 법사 복장세트'       then '50730,180,70020,-1'
                            when '가디언나이츠 도적 복장세트'       then '50800,180,70020,-1'
                            when '가디언나이츠 드루이드 복장세트'   then '50870,180,70020,-1'
                            when '가디언나이츠 마검사 복장세트'     then '70010,180,70020,-1'
                            --071129. microcat. 가이드북2 - 5,100
                            --080402. microcat. 코믹북 16권
                            when '080402 여왕의보석 42410 7일'  then '42410,7'
                            when '080402 No1 31720 30일'        then '31720,30'
                            when '080402 수호의반지 12710 60회' then '12710,60'
                            when '080402 잼 8880 100개'         then '8880,100'
                            when '080402 GC스타 30470 30일'     then '30470,30'
                            when '080402 절대반지 40 30일'      then '40,30'
                            --080402. microcat. 코믹북 16권
                            --080411. microcat. 뮤지컬
                            when '080411 카나반의증표 영구'     then '80410,-1'
                            when '080411 카나반이모티콘 영구'   then '80420,1'
                            when '080411 카나반의날개 영구'     then '80430,-1'
                            when '080411 카나반의망또 영구'     then '79430,-1,79440,-1,79450,-1,79460,-1,79470,-1,79480,-1,'
                            --080411. microcat. 뮤지컬
                            --080514. microcat. 가이드북
                            when '080514 카니발기사복장세트 180일'      then '74510,180,31730,30,37450,90'
                            when '080514 카니발궁수복장세트 180일'      then '74520,180,31730,30,37450,90'
                            when '080514 카니발마법사복장세트 180일'    then '74530,180,31730,30,37450,90'
                            when '080514 카니발도적복장세트 180일'      then '74540,180,31730,30,37450,90'
                            when '080514 카니발드루이드복장세트 180일'  then '74550,180,31730,30,37450,90'
                            when '080514 카니발마검사복장세트 180일'    then '74650,180,31730,30,37450,90'
                            --080514. microcat. 가이드북
                            --080721. mk8253. 코믹북 17권
                            when '080721 닌코 봉인카드 영구'    then '42350,-1,'
                            when '080721 GC 스타 15일'          then '30470,15,'
                            when '080721 수호의 반지 60회'      then '12710,60,'
                            when '080721 수호의 반지 30회'      then '51300,30,'
                            when '080721 +30 가방 1회'          then '43500,1,'
                            when '080721 절대반지 15일'         then '40,15,'
                            --080721. mk8253. 코믹북 17권
                            --081027. mk8253. 코믹북 18권
                            when '081027 큐피트 봉인카드 영구'      then '79050,-1,82050,10,'
                            when '081027 GC 스타 30일'              then '30470,30,82050,10,'
                            when '081027 명예의 금반지 30일'        then '89630,30,82050,10,'
                            when '081027 강화율 상승 아이템 3회'    then '71770,3,82050,10,'
                            when '081027 이지스 방패 30일'          then '88460,30,82050,10,'
                            when '081027 절대 반지 30일'            then '40,30,82050,10,'
                            --081027. mk8253. 코믹북 18권
                            --081110. mk8253. 코믹북 18권 추가
                            when '081110 큐피트 봉인카드 영구'      then '79050,-1,82050,10,'
                            when '081110 GC 스타 30일'              then '30470,30,82050,10,'
                            when '081110 명예의 금반지 30일'        then '89630,30,82050,10,'
                            when '081110 강화율 상승 아이템 3회'    then '71770,3,82050,10,'
                            when '081110 이지스 방패 30일'          then '88460,30,82050,10,'
                            when '081110 절대 반지 30일'            then '40,30,82050,10,'
                            --081110. mk8253. 코믹북 18권 추가
                            --081110. mk8253. 코믹북 17권 추가
                            when '081110 닌코 봉인카드 영구'    then '42350,-1,'
                            when '081110 GC 스타 15일'          then '30470,15,'
                            when '081110 수호의 반지 60회'      then '12710,60,'
                            when '081110 수호의 반지 30회'      then '51300,30,'
                            when '081110 +30 가방 1회'          then '43500,1,'
                            when '081110 절대반지 15일'         then '40,15,'
                            --081110. mk8253. 코믹북 17권 추가
                            --081111. mk8253. 뮤지컬
                            when '081111 GC클럽 30일'    then '42340,30,40,30,'
                            --081111. mk8253. 뮤지컬
                            --081112. mk8253. 코믹북 16권
                            when '081112 여왕의보석 42410 7일'  then '42410,7'
                            when '081112 No1 31720 30일'        then '31720,30'
                            when '081112 수호의반지 12710 60회' then '12710,60'
                            when '081112 잼 8880 100개'         then '8880,100'
                            when '081112 GC스타 30470 30일'     then '30470,30'
                            when '081112 절대반지 40 30일'      then '40,30'
                            --081112. mk8253. 코믹북 16권
                            --081112. mk8253. 코믹북 10권
                            when '081112 태양 머리 장식 30일'       then '30790,30,'
                            when '081112 태양 날개 장식 30일'       then '30800,30,'
                            when '081112 태양 팔 장식 30일'         then '30810,30,'
                            when '081112 용사의 반지 30일'          then '29920,30,'
                            when '081112 카제아제 힐링 볼 200회'    then '17470,200,'
                            when '081112 공작 깃털 날개 100회'      then '16690,100,'
                            --081112. mk8253. 코믹북 10권
                            --090115. mk8253. 오프라인대회
                            when '090115 오프라인대회' then '88460,30,42340,30,'
                            --090115. mk8253. 오프라인대회

                            --20090119. microcat. 코믹북15권
                            when '20090119 닌코 봉인카드 42350 -1'     then '42350,-1,'
                            when '20090119 GC 스타 (30일) 30470 30'    then '30470,30,'
                            when '20090119 크라운스타(30일) 31730 30'  then '31730,30,'
                            when '20090119 수호의 반지(60회) 12710 60' then '12710,60,'
                            when '20090119 +60 가방(1회) 42220 1'      then '42220,1,'
                            when '20090119 절대반지(30일) 40 30'       then '40,30,'
                            --20090119. microcat. 코믹북15권

                        END
            END
        ELSE IF @iCharType_ BETWEEN 0 AND 3 AND @strItemName = N'GC공략집'
            BEGIN
                SELECT  @iGoodsID = 33550 + 10 * @iCharType_
,                       @iDuration = -1
,                       @strSQL =
                        CASE @iCharType_
                            WHEN 0 THEN N'33390,-1,33400,-1,33410,-1,33420,-1,33220,-1'
                            WHEN 1 THEN N'33430,-1,33440,-1,33450,-1,33460,-1,33220,-1'
                            WHEN 2 THEN N'33470,-1,33480,-1,33490,-1,33500,-1,33220,-1'
                            WHEN 3 THEN N'33510,-1,33520,-1,33530,-1,33540,-1,33220,-1'
                        END
,                       @strItemName =
                        CASE @iCharType_
                            WHEN 0 THEN N'엘리시스 GC공략집'
                            WHEN 1 THEN N'리르 GC공략집'
                            WHEN 2 THEN N'아르메 GC공략집'
                            WHEN 3 THEN N'라스 GC공략집'
                        END
            END
        ELSE
            BEGIN    SELECT @iOK = -5    GOTO END_PROC    END

        IF @strSQL is NULL
            BEGIN    SELECT @iOK = -6    GOTO END_PROC    END

        BEGIN TRAN
            EXEC @iOK = dbo.up_insert_item @strLogin_, @strSQL, ',', 0
            IF @iOK <> 0
                GOTO FAIL_TRAN
    END
ELSE
    BEGIN
        SELECT @iGoodsID = -2, @iDuration = 5000
        BEGIN TRAN
            UPDATE dbo.Users WITH (UPDLOCK)
                SET GamePoint = dbo.zcGamePoint(GamePoint + @iDuration)
                    WHERE Login = @strLogin_
            IF @@ERROR <> 0
                GOTO FAIL_TRAN
    END

    UPDATE dbo.ComicBookItemInfo WITH (ROWLOCK)
        SET bUse = 1
            WHERE SerialNumber = @strSN_
    IF @@ERROR <> 0
        GOTO FAIL_TRAN

    INSERT INTO dbo.ComicBookReceiveUser ( SerialNumber, ItemName, UserLogin, RegDate ) 
      VALUES ( @strSN_, @strItemName, @strLogin_, GetDate() )
    IF @@ERROR <> 0
    GOTO FAIL_TRAN
COMMIT TRAN

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN
SELECT @iOK = -9

END_PROC:
IF @iOK <> 0
    BEGIN    SELECT @iGoodsID = -1, @iDuration = -1    END

SELECT @iOK, @iGoodsID, @iDuration


