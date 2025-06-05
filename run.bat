@echo off
cd /d "%~dp0"

REM ===== 1. Clean & Rebuild =====
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
REM ⚠️ Update this path to your actual MinGW bin folder
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
powershell -Command "Write-Host 'Build and package completed. Files are in /dist' -ForegroundColor Green"
echo.
pause
