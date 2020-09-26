CREATE FUNCTION [dbo].[fnLevelToExp](@iLevel [int])
RETURNS [bigint] WITH SCHEMABINDING
AS 
BEGIN
    DECLARE @iExp bigint
    SELECT @iExp = CASE @iLevel
                        WHEN 0  THEN 2000
                        WHEN 1  THEN 2020
                        WHEN 2  THEN 2200
                        WHEN 3  THEN 2500
                        WHEN 4  THEN 2920
                        WHEN 5  THEN 3460
                        WHEN 6  THEN 4060
                        WHEN 7  THEN 4720
                        WHEN 8  THEN 5440
                        WHEN 9  THEN 6220
                        WHEN 10 THEN 7500
                        WHEN 11 THEN 9400
                        WHEN 12 THEN 11700
                        WHEN 13 THEN 14940
                        WHEN 14 THEN 19280
                        WHEN 15 THEN 25580
                        WHEN 16 THEN 34160
                        WHEN 17 THEN 45600
                        WHEN 18 THEN 59320
                        WHEN 19 THEN 75520
                        WHEN 20 THEN 96760
                        WHEN 21 THEN 122360
                        WHEN 22 THEN 152180
                        WHEN 23 THEN 184940
                        WHEN 24 THEN 220640
                        WHEN 25 THEN 259280
                        WHEN 26 THEN 306800
                        WHEN 27 THEN 391600
                        WHEN 28 THEN 527200
                        WHEN 29 THEN 827200
                        WHEN 30 THEN 1208200
                        WHEN 31 THEN 1744200
                        WHEN 32 THEN 2550600
                        WHEN 33 THEN 3536200
                        WHEN 34 THEN 4848200
                        WHEN 35 THEN 7110200
                        WHEN 36 THEN 10054200
                        WHEN 37 THEN 13740200
                        WHEN 38 THEN 18228200
                        WHEN 39 THEN 23578200
                        WHEN 40 THEN 29850200
                        WHEN 41 THEN 37338200
                        WHEN 42 THEN 46194200
                        WHEN 43 THEN 56514200
                        WHEN 44 THEN 68394200
                        WHEN 45 THEN 81930200
                        WHEN 46 THEN 97218200
                        WHEN 47 THEN 114354200
                        WHEN 48 THEN 133434200
                        WHEN 49 THEN 154554200
                        WHEN 50 THEN 177810200
                        WHEN 51 THEN 203298200
                        WHEN 52 THEN 231342200
                        WHEN 53 THEN 262062200
                        WHEN 54 THEN 295578200
                        WHEN 55 THEN 332010200
                        WHEN 56 THEN 371478200
                        WHEN 57 THEN 414582200
                        WHEN 58 THEN 461482200
                        WHEN 59 THEN 512338200
                        WHEN 60 THEN 567310200
                        WHEN 61 THEN 628674200
                        WHEN 62 THEN 695154200
                        WHEN 63 THEN 764634200
                        WHEN 64 THEN 837114200
                        WHEN 65 THEN 912594200
                        WHEN 66 THEN 991074200
                        WHEN 67 THEN 1073154200
                        WHEN 68 THEN 1158834200
                        WHEN 69 THEN 1248114200
                        WHEN 70 THEN 1340994200
                        WHEN 71 THEN 1437474200
                        WHEN 72 THEN 1538154200
                        WHEN 73 THEN 1643034200
                        WHEN 74 THEN 1752114200
                        WHEN 75 THEN 1865394200
                        ELSE 2000 END

    RETURN @iExp
END


