@echo off
setlocal

set "SOLUTION_PATH=%~1"
set "MSBUILD_PATH=%~2"

if "%SOLUTION_PATH%"=="" (echo Missing SOLUTION_PATH\nUsage: build_solution.bat {SOLUTION_PATH} {MSBUILD_PATH} & exit /b 1)
if "%MSBUILD_PATH%"=="" (echo Missing MSBUILD_PATH\nUsage: build_solution.bat {SOLUTION_PATH} {MSBUILD_PATH} & exit /b 1)

echo Running MSBuild on "%SOLUTION_PATH%"
"%MSBUILD_PATH%" "%SOLUTION_PATH%" /p:Configuration=Release

endlocal