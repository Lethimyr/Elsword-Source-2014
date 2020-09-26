CREATE FUNCTION [dbo].[zbExp_Level](@iInput [int])
RETURNS [bigint] WITH SCHEMABINDING
AS 
begin
    declare @iOutput bigint

    select @iOutput =
        case @iInput
            when 0  then 2000
            when 1  then 2020
            when 2  then 2200
            when 3  then 2500
            when 4  then 2920
            when 5  then 3460
            when 6  then 4060
            when 7  then 4720
            when 8  then 5440
            when 9  then 6220
            when 10 then 7500
            when 11 then 9400
            when 12 then 11700
            when 13 then 14940
            when 14 then 19280
            when 15 then 25580
            when 16 then 34160
            when 17 then 45600
            when 18 then 59320
            when 19 then 75520
            when 20 then 96760
            when 21 then 122360
            when 22 then 152180
            when 23 then 184940
            when 24 then 220640
            when 25 then 259280
            when 26 then 306800
            when 27 then 391600
            when 28 then 527200
            when 29 then 827200
            when 30 then 1208200
            when 31 then 1744200
            when 32 then 2550600
            when 33 then 3536200
            when 34 then 4848200
            when 35 then 7110200
            when 36 then 10054200
            when 37 then 13740200
            when 38 then 18228200
            when 39 then 23578200
            when 40 then 29850200
            when 41 then 37338200
            when 42 then 46194200
            when 43 then 56514200
            when 44 then 68394200
            when 45 then 81930200
            when 46 then 97218200
            when 47 then 114354200
            when 48 then 133434200
            when 49 then 154554200
            when 50 then 177810200
            when 51 then 203298200
            when 52 then 231342200
            when 53 then 262062200
            when 54 then 295578200
            when 55 then 332010200
            when 56 then 371478200
            when 57 then 414582200
            when 58 then 461482200
            when 59 then 512338200
            when 60 then 567310200
            when 61 then 628674200
            when 62 then 695154200
            when 63 then 764634200
            when 64 then 837114200
            when 65 then 912594200
            when 66 then 991074200
            when 67 then 1073154200
            when 68 then 1158834200
            when 69 then 1248114200
            when 70 then 1340994200
            when 71 then 1437474200
            when 72 then 1538154200
            when 73 then 1643034200
            when 74 then 1752114200
            when 75 then 1865394200
            else 2000
        end

    return @iOutput
END


