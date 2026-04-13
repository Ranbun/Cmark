@echo off
setlocal

set BUILD_DIR=out\build\windows_debug

echo Cleaning previous build directory...
if exist "%BUILD_DIR%" (
    rmdir /s /q "%BUILD_DIR%"
)
mkdir "%BUILD_DIR%"

echo.
echo [1/2] Configuring CMake project...
cmake -S . -B "%BUILD_DIR%" -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
if %errorlevel% neq 0 (
    echo CMake configuration failed!
    exit /b %errorlevel%
)

echo.
echo [2/2] Building the project (Release)...
cmake --build "%BUILD_DIR%" --config Release --parallel %NUMBER_OF_PROCESSORS%
if %errorlevel% neq 0 (
    echo Build failed!
    exit /b %errorlevel%
)

echo.
echo Build completed successfully. The executable (CameraMark.exe) should be located inside the "%BUILD_DIR%\src\Release" or "%BUILD_DIR%\src" directory.

echo.
if exist "%BUILD_DIR%\compile_commands.json" (
    echo Copying compile_commands.json to the project root for clangd...
    copy /Y "%BUILD_DIR%\compile_commands.json" "compile_commands.json" >nul
) else (
    echo [Warning] compile_commands.json was not generated. This might be because you are using an older CMake ^(< 3.27^) with a Visual Studio Generator.
)

endlocal
