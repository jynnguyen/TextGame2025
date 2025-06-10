@echo off
cd /d "%~dp0"
cls
REM ===== 1. Clean & Rebuild =====
powershell -Command "`Write-Host ' > Building project with CMake... ' -ForegroundColor Yellow"
if exist build (
    rmdir /s /q build
)
mkdir build
cd build
powershell -Command "Write-Host ' [CMAKE] Configuring project...' -ForegroundColor Blue"
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..

powershell -Command "Write-Host ' [CMAKE] Building project...' -ForegroundColor Blue"
cmake --build .
powershell -Command "Write-Host ' >> Build completed.' -ForegroundColor Green"
echo.

cd ..

REM ===== 2. Prepare Distribution Folder =====
powershell -Command "Write-Host ' [PACKAGE] Preparing dist folder...' -ForegroundColor Blue"
if exist dist (
    rmdir /s /q dist
)

mkdir dist
copy build\play.exe dist\
mkdir dist\Data
copy Data dist\Data\

REM ===== 3. Copy Required DLLs (from MinGW bin folder) =====
set MINGW_BIN="C:\msys64\ucrt64\bin"

powershell -Command "Write-Host ' [PACKAGE] Copying DLLs...' -ForegroundColor Blue"
copy %MINGW_BIN%\libstdc++-6.dll dist\
copy %MINGW_BIN%\libgcc_s_seh-1.dll dist\
copy %MINGW_BIN%\libwinpthread-1.dll dist\

REM ===== 5. Done =====
powershell -Command "Write-Host ' >> Package completed.' -ForegroundColor Green"
powershell -Command "Write-Host ' Files are in dist folder' -ForegroundColor Cyan"
set /p answer=Do you want to run play.exe? (y/n): 

if /i "%answer%"=="y" (
    powershell -Command "Write-Host ' > Running play.exe file' -ForegroundColor Yellow"
    start "" "dist\play.exe"
) else (
    powershell -Command "Write-Host ' > Cancel running ' -ForegroundColor Red"
)

pause

