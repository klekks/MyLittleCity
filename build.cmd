
mkdir .\build\distr

@echo off

set CONFIG=Release
set ARCH=x86_64

IF "%~1" NEQ "" set CONFIG=%~1
IF "%~2" NEQ "" set ARCH=%~2
IF "%~3" EQU "SkipAllConan" goto SkipAllConan
IF "%~3" EQU "NoClean" goto NoClean
IF "%~3" EQU "SkipConanButConfig" goto SkipConanButConfig

echo Removing build
rd /s /q build

if %ERRORLEVEL% NEQ 0 (
    echo ERROR Remove build Failed with exit-code: %errorlevel%
    exit /b %errorlevel%
)

echo build folders removed

echo Removing old conan packages

conan remove -c *
if %ERRORLEVEL% NEQ 0 (
    echo ERROR Remove conan Failed with exit-code: %errorlevel%
    exit /b %errorlevel%
)

conan profile detect --force
copy conan\win_conan_profile %userprofile%\.conan2\profiles\default

if %ERRORLEVEL% NEQ 0 (
    echo ERROR install conan profile Failed with exit-code: %errorlevel%
    exit /b %errorlevel%
)

:NoClean

echo Conan profile installed

conan install . --build=missing -s build_type=%CONFIG% -s arch=%ARCH%

:SkipConanButConfig

:SkipAllConan

echo Conan build steps skipped

echo Generate make files

cd build

cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=%CONFIG% -DCMAKE_TOOLCHAIN_FILE="%CONFIG%/generators/conan_toolchain.cmake"

if %ERRORLEVEL% NEQ 0 (
    echo ERROR Cannot generate make files with exit-code: %errorlevel%
    exit /b %errorlevel%
)

cmake --build . --config %CONFIG% -j 8

if %ERRORLEVEL% NEQ 0 (
    echo ERROR Cannot build with exit-code: %errorlevel%
    exit /b %errorlevel%
)

echo Build Successful!
