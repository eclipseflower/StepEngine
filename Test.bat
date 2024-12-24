@REM rd /s /q Build
@REM md Build
cmake -S Source -B Build -G "Unix Makefiles"
cmake --build Build
@cd Build
ctest -R Matrix4x4Test
@cd ..
