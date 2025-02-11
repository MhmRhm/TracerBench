add_executable(app app.cpp)

BuildInfo(app)
CleanCoverage(app)
Format(app .)
AddCppcheck(app)
Doxygen(app src/app)

target_compile_options(app
	PRIVATE ${DEFAULT_CXX_COMPILE_FLAGS}
	PRIVATE ${DEFAULT_CXX_OPTIMIZE_FLAG}
)

target_link_libraries(app
	PRIVATE libapp
)
