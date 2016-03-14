@ECHO OFF

REM Copyright 2016 Intel Corporation All Rights Reserved
REM 
REM Intel makes no representations about the suitability of this software for any purpose.
REM THIS SOFTWARE IS PROVIDED ""AS IS."" INTEL SPECIFICALLY DISCLAIMS ALL WARRANTIES,
REM EXPRESS OR IMPLIED, AND ALL LIABILITY, INCLUDING CONSEQUENTIAL AND OTHER INDIRECT DAMAGES,
REM FOR THE USE OF THIS SOFTWARE, INCLUDING LIABILITY FOR INFRINGEMENT OF ANY PROPRIETARY
REM RIGHTS, AND INCLUDING THE WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
REM Intel does not assume any responsibility for any errors which may appear in this software
REM nor any responsibility to update it.

echo Preparing 'IntroductionToVulkan' solution...

mkdir build
cd build

cmake.exe .. -DUSE_PLATFORM=VK_USE_PLATFORM_WIN32_KHR -G "Visual Studio 12 2013 Win64"

start "" "IntroductionToVulkan.sln"

cd ..