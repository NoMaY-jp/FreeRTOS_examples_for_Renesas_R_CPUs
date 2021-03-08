@echo off
rem ###############################################################################################
rem # This batch file executes some preprocess for build and then executes the make if necessary
rem ###############################################################################################

setlocal enabledelayedexpansion

set verbose=0

if exist ".\%~nx0" (
    set ide=csp
    set pj=.\
    rem the following is CCRL only
    set tc=CCRL
) else if exist "..\%~nx0" (
    set ide=e2
    set pj=..\
    rem the following is GNUC or CCRL
    set tc=GNUC
) else (
    echo\
    echo Error: Unable to find the project folder.
    goto ERROR
)

if %~n0==build_for_cpp_project (set cpp=true) else (set cpp=false)
if %ide%==e2 if %tc%==CCRL set TOOLCHAIN_TOOLS_CHECK=make.exe ccrl.exe
if %ide%==e2 if %tc%==GNUC set TOOLCHAIN_TOOLS_CHECK=make.exe rl78-elf-gcc.exe

set cp=Active code page: 437
for /f "delims==" %%P in ('chcp') do if not "%%P"=="" (set cp=%%P)
if "%cp:~-4%"==" 932%" (set site=ja) else (set site=en)

if %verbose%==1 echo IDE is %ide%
if %verbose%==1 echo Project Folder is %pj%
if %verbose%==1 echo Tool Chain is %tc%
if %verbose%==1 echo C++ mode is %cpp%
if %verbose%==1 echo TOOLCHAIN_TOOLS_CHECK=%TOOLCHAIN_TOOLS_CHECK%
if %verbose%==1 echo site is %site%-support.renesas.com

if %ide%==e2 (
    for %%S in (%TOOLCHAIN_TOOLS_CHECK%) do (
        if "%%~$PATH:S"=="" (
            rem FAILED
            if %verbose%==1 echo %%S cannot be found in PATH=%PATH%
            echo\
            echo [ERROR] No toolchain set or toolchain not integrated.
            echo\
            echo Please refer to the following Knowledge Base articles to solve this problem.
            echo https://%site%-support.renesas.com/knowledgeBase/18367361
            echo https://%site%-support.renesas.com/knowledgeBase/17797630
            goto ERROR
        )
    )
)

if %ide%==e2 if "%1"=="clean" goto CLEAN
if %ide%==e2 if "%2"=="clean" goto CLEAN
if %ide%==e2 if "%3"=="clean" goto CLEAN
if %ide%==e2 if "%4"=="clean" goto CLEAN
if %ide%==e2 if "%5"=="clean" goto CLEAN
if %ide%==e2 if "%6"=="clean" goto CLEAN
if %ide%==e2 if "%7"=="clean" goto CLEAN
if %ide%==e2 if "%8"=="clean" goto CLEAN
if %ide%==e2 if "%9"=="clean" goto CLEAN

:MODIFY_AND_TOUCH_FILES
if %tc%==GNUC if exist "%pj%src\" (
    echo\
    echo 'Checking source code tree...'
    set ef=0
    for %%S in (r_cg_vector_table.js r_cg_vector_table.c r_cg_vector_table.h) do (
        if not exist "%pj%\src\%%S" (
            rem FAILED
            if !ef!==0 (
                set ef=1
                echo\
            )
            echo Missing src\%%S
        )
        if !ef!==1 (
            echo\
            goto ERROR
        )
    )
    for %%T in ("%pj%\src\r_cg_vector_table.c") do set C_DATETIME=%%~tT
    for %%T in ("%pj%\src\r_cg_vector_table.h") do set H_DATETIME=%%~tT
    if %verbose%==1 echo r_cg_vector_table.c date time=!C_DATETIME!
    if %verbose%==1 echo r_cg_vector_table.h date time=!H_DATETIME!
    if "!H_DATETIME!" LSS "!C_DATETIME!" (
        echo ' '
        echo 'Modifying file: ../src/r_cg_vector_table.c'
        echo 'Touching file: ../src/r_cg_vector_table.h'
        cscript -nologo "%pj%\src\r_cg_vector_table.js" "%pj%\src" r_cg_vector_table.c r_cg_vector_table.h
    )
)

if %ide%==e2 goto MAKE_ALL

endlocal
exit 0

:MAKE_ALL
endlocal
echo\
echo 'Invoking make...'
echo ' '
set BUILDARTIFACTDATETIME=
if exist "%BUILDARTIFACTFILENAME%" for %%T in ("%BUILDARTIFACTFILENAME%") do (
    set BUILDARTIFACTDATETIME=%%~tT
)
make -r %*
if exist "%BUILDARTIFACTFILENAME%" for %%T in ("%BUILDARTIFACTFILENAME%") do (
    if not "%%~tT"=="%BUILDARTIFACTDATETIME%" goto SHOW_SIZE
)
exit %ERRORLEVEL%

:SHOW_SIZE
echo 'Invoking size command'
echo rl78-elf-size "%BUILDARTIFACTFILENAME%"
rl78-elf-size "%BUILDARTIFACTFILENAME%"
exit %ERRORLEVEL%

:CLEAN
endlocal
make -r %*
exit %ERRORLEVEL%

:ERROR
endlocal
exit 2
