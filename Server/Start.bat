@echo off

cd "C:\MuGoblins\Server"

cd ConnectServer
start ./ConnectServer.exe

timeout 1 > NUL

cd ../DataServer
start ./DataServer.exe

timeout 1 > NUL

cd ../JoinServer
start ./JoinServer.exe

timeout 1 > NUL

cd ../GameServer
start ./GameServer.exe

timeout 1 > NUL

cd ../GameServerCS
start ./GameServer.exe