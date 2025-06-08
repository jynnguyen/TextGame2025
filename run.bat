@echo off
cd /d "%~dp0"
cls
REM ===== 1. Clean & Rebuild =====
powershell -Command "Write-Host ' > Building project with CMake... ' -ForegroundColor Yellow"
if exist build (
    rmdir /s /q build
)
mkdir build
cd build
echo.
echo [CMAKE] Configuring project...
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..

echo [CMAKE] Building project...
cmake --build .
echo.
powershell -Command "Write-Host ' >> Build completed.' -ForegroundColor Green"
echo.

cd ..

REM ===== 2. Prepare Distribution Folder =====
echo [PACKAGE] Preparing dist folder...
if exist dist (
    rmdir /s /q dist
)

mkdir dist
copy build\play.exe dist\
mkdir dist\Data
copy Data dist\Data\

REM ===== 3. Copy Required DLLs (from MinGW bin folder) =====
set MINGW_BIN="C:\msys64\ucrt64\bin"

echo [PACKAGE] Copying DLLs...
copy %MINGW_BIN%\libstdc++-6.dll dist\
copy %MINGW_BIN%\libgcc_s_seh-1.dll dist\
copy %MINGW_BIN%\libwinpthread-1.dll dist\

REM ===== 4. Strip Debug Info =====
echo [STRIP] Removing debug symbols...
%MINGW_BIN%\strip dist\play.exe

REM ===== 5. Done =====
echo.
powershell -Command "Write-Host ' >> Package completed.' -ForegroundColor Green"
powershell -Command "Write-Host ' Files are in dist folder' -ForegroundColor Cyan"
echo.
pause
