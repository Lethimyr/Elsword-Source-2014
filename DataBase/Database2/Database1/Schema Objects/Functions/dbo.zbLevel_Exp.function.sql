CREATE FUNCTION [dbo].[zbLevel_Exp](@iInput [bigint])
RETURNS [tinyint] WITH SCHEMABINDING
AS 
begin
    declare @iOutput tinyint

    select @iOutput =
        case
            when @iInput < 2020       then 0
            when @iInput < 2200       then 1
            when @iInput < 2500       then 2
            when @iInput < 2920       then 3
            when @iInput < 3460       then 4
            when @iInput < 4060       then 5
            when @iInput < 4720       then 6
            when @iInput < 5440       then 7
            when @iInput < 6220       then 8
            when @iInput < 7500       then 9
            when @iInput < 9400       then 10
            when @iInput < 11700      then 11
            when @iInput < 14940      then 12
            when @iInput < 19280      then 13
            when @iInput < 25580      then 14
            when @iInput < 34160      then 15
            when @iInput < 45600      then 16
            when @iInput < 59320      then 17
            when @iInput < 75520      then 18
            when @iInput < 96760      then 19
            when @iInput < 122360     then 20
            when @iInput < 152180     then 21
            when @iInput < 184940     then 22
            when @iInput < 220640     then 23
            when @iInput < 259280     then 24
            when @iInput < 306800     then 25
            when @iInput < 391600     then 26
            when @iInput < 527200     then 27
            when @iInput < 827200     then 28
            when @iInput < 1208200    then 29
            when @iInput < 1744200    then 30
            when @iInput < 2550600    then 31
            when @iInput < 3536200    then 32
            when @iInput < 4848200    then 33
            when @iInput < 7110200    then 34
            when @iInput < 10054200   then 35
            when @iInput < 13740200   then 36
            when @iInput < 18228200   then 37
            when @iInput < 23578200   then 38
            when @iInput < 29850200   then 39
            when @iInput < 37338200   then 40
            when @iInput < 46194200   then 41
            when @iInput < 56514200   then 42
            when @iInput < 68394200   then 43
            when @iInput < 81930200   then 44
            when @iInput < 97218200   then 45
            when @iInput < 114354200  then 46
            when @iInput < 133434200  then 47
            when @iInput < 154554200  then 48
            when @iInput < 177810200  then 49
            when @iInput < 203298200  then 50
            when @iInput < 231342200  then 51
            when @iInput < 262062200  then 52
            when @iInput < 295578200  then 53
            when @iInput < 332010200  then 54
            when @iInput < 371478200  then 55
            when @iInput < 414582200  then 56
            when @iInput < 461482200  then 57
            when @iInput < 512338200  then 58
            when @iInput < 567310200  then 59
            when @iInput < 628674200  then 60
            when @iInput < 695154200  then 61
            when @iInput < 764634200  then 62
            when @iInput < 837114200  then 63
            when @iInput < 912594200  then 64
            when @iInput < 991074200  then 65
            when @iInput < 1073154200 then 66
            when @iInput < 1158834200 then 67
            when @iInput < 1248114200 then 68
            when @iInput < 1340994200 then 69
            when @iInput < 1437474200 then 70
            when @iInput < 1538154200 then 71
            when @iInput < 1643034200 then 72
            when @iInput < 1752114200 then 73
            when @iInput < 1865394200 then 74
            else 75
        end

    return @iOutput
end


