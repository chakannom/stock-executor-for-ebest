# Stock-Executor for EBEST

* This is a tool to call API of [EBEST (eBEST INVESTMENT SECURITIES Co.,Ltd.)](https://www.ebestsec.co.kr) by request received from agent.
* This application runs on only Windows.

## Development

* You must build on only Windows and for x86. If you install Visual Studio, you can develop it easily.

Before you can build this project, you must install and configure the following dependencies on your machine:

1. [MSVC](https://visualstudio.microsoft.com): We use MSVC to run a development and build the project.
   You can install MSVC from <https://visualstudio.microsoft.com/ko/downloads>.
   You must select a workload `Desktop development with C++` and a component `C++ MFC for latest v142(or higher) build tools (x86 and x64)`.
2. [vcpkg](https://github.com/microsoft/vcpkg): We use vcpkg to manage package dependencies for C++.
   You can install vcpkg from <https://github.com/microsoft/vcpkg#quick-start-windows>.

### Package dependencies

* cpprestsdk:x86-windows (vcpkg): <https://github.com/microsoft/cpprestsdk#getting-started>

## Building

To optimize the stock-executor-for-ebest application, run:

for Debugging
```
MSBuild.exe stock-executor-for-ebest.vcxproj /p:Configuration=Debug
```

for Release

```
MSBuild.exe stock-executor-for-ebest.vcxproj /p:Configuration=Release
```

To ensure everything worked, run:

for Debugging
```
stock-executorr-for-ebest.exe
```

for Release

```
stock-executor-for-ebest.exe --execute uuid
```