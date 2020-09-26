rem @echo off
:Loop
p4 sync //depot/X2Project/KncWX2Server/ServerPatchExe/CenterServer.*
..\ServerPatchEXE\CenterServer.exe NEXON_KOREA
goto Loop


