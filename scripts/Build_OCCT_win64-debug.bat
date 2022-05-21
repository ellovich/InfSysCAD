:: TODO check if there is no installed cmake & give a link: https://cmake.org/download/

@echo off
cd ..\InfSysCAD\products\
@echo on

cmake -S occt -B occt-build^
 -DCMAKE_BUILD_TYPE=debug^
 -DBUILD_MODULE_ApplicationFramework=ON^
 -DBUILD_MODULE_DataExchange=ON^
 -DBUILD_MODULE_Draw=ON^
 -DBUILD_MODULE_FoundationClasses=ON^
 -DBUILD_MODULE_ModelingAlgorithms=ON^
 -DBUILD_MODULE_ModelingData=ON^
 -DBUILD_MODULE_Visualization=ON^
 -DINSTALL_DIR=occt-install^
 -DUSE_FREETYPE=ON^
 -DUSE_FFMPEG=OFF^
 -DUSE_VTK=OFF^
 -DUSE_TBB=OFF^
 -D3RDPARTY_DIR=..\..\products^
 && "C:\Program Files\Microsoft Visual Studio\2022\Community\Msbuild\Current\Bin\MSBuild.exe" -nologo occt-build/INSTALL.vcxproj /p:config=Debug /p:platform=x64^
 && echo OpenCascade built succsessful!

@echo off
cd ..\..\scripts
pause
@echo on



:::: TODO check if there is no installed cmake & give a link: https://cmake.org/download/
::
::@echo off
::cd ..\InfSysCAD\products\
::@echo on
::
::cmake -S occt -B occt-build^
:: -DCMAKE_BUILD_TYPE=debug^
:: -DBUILD_MODULE_ApplicationFramework=OFF^
:: -DBUILD_MODULE_DataExchange=OFF^
:: -DBUILD_MODULE_Draw=OFF^
:: -DBUILD_MODULE_FoundationClasses=OFF^
:: -DBUILD_MODULE_ModelingAlgorithms=OFF^
:: -DBUILD_MODULE_ModelingData=OFF^
:: -DBUILD_MODULE_Visualization=OFF^
:: -DBUILD_ADDITIONAL_TOOLKITS="TKernel;TKOpenGl;TKPrim;TKService;TKSTEP;TKTopAlgo;TKV3d;TKXSBase;TKMath;TKBRep;TKGeomBase;TKG3d;TKG2d;TKSTEPAttr;TKSTEP209;TKSTEPBase;TKShHealing;TKGeomAlgo;TKMesh;TKHLR"^
:: -DINSTALL_DIR=occt-install^
:: -DUSE_FREETYPE=ON^
:: -DUSE_FFMPEG=OFF^
:: -DUSE_VTK=OFF^
:: -DUSE_TBB=OFF^
:: -D3RDPARTY_DIR=..\..\products^
:: && "C:\Program Files\Microsoft Visual Studio\2022\Community\Msbuild\Current\Bin\MSBuild.exe" -nologo occt-build/INSTALL.vcxproj /p:config=Debug /p:platform=x64^
:: && echo OpenCascade built succsessful!
::
::@echo off
::cd ..\..\scripts
::pause
::@echo on
