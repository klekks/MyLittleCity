
mkdir .\builds\distr

@echo off

set CONFIG=Release
set ARCH=x86_64

IF "%~1" NEQ "" set CONFIG=%~1
IF "%~2" NEQ "" set ARCH=%~2
IF "%~3" EQU "SkipAllConan" goto SkipAllConan
IF "%~3" EQU "NoClean" goto NoClean
IF "%~3" EQU "SkipConanButConfig" goto SkipConanButConfig

echo Removing builds
rd /s /q builds

if %ERRORLEVEL% NEQ 0 (
    echo ERROR Remove build Failed with exit-code: %errorlevel%
    exit /b %errorlevel%
)

echo builds folders removed

echo Removing old conan packages

conan remove -c *
if %ERRORLEVEL% NEQ 0 (
    echo ERROR Remove conan Failed with exit-code: %errorlevel%
    exit /b %errorlevel%
)

echo Conan folder removed
mkdir %userprofile%\.conan2\profiles
copy conan_profile %userprofile%\.conan2\profiles\default

if %ERRORLEVEL% NEQ 0 (
    echo ERROR install conan profile Failed with exit-code: %errorlevel%
    exit /b %errorlevel%
)

echo Conan profile installed

:NoClean

echo Starting to build dependencies

conan install . --output-folder=builds/builds_conan/%ARCH%/%CONFIG% -s build_type=%CONFIG% -s arch=%ARCH% --build=missing

if %ERRORLEVEL% NEQ 0 (
    echo ERROR Cannot compile base lib Failed with exit-code: %errorlevel%
    exit /b %errorlevel%
)

echo Base libs compiled

:SkipConanButConfig

conan install . --output-folder=builds/builds_conan/%ARCH%/%CONFIG% -s build_type=%CONFIG% -s arch=%ARCH%
if %ERRORLEVEL% NEQ 0 (
    echo ERROR Cannot compile base lib Failed with exit-code: %errorlevel%
    exit /b %errorlevel%
)


:SkipAllConan

echo Conan build steps skipped

echo Generate make files

cmake -B builds/%ARCH%/%CONFIG% -G "MinGW Makefiles" -DCMAKE_TOOLCHAIN_FILE=builds/builds_conan/%ARCH%/%CONFIG%/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=%CONFIG%

if %ERRORLEVEL% NEQ 0 (
    echo ERROR Cannot generate make files with exit-code: %errorlevel%
    exit /b %errorlevel%
)


cmake --build builds/%ARCH%/%CONFIG% --config %CONFIG% -j 8

if %ERRORLEVEL% NEQ 0 (
    echo ERROR Cannot build with exit-code: %errorlevel%
    exit /b %errorlevel%
)

echo Build Successful!
