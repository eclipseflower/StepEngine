@REM rd /s /q Build
@REM md Build
cmake -S Source -B Build -G "Unix Makefiles"
cmake --build Build
@cd Build
ctest
@cd ..
