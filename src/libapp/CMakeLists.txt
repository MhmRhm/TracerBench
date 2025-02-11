add_library(libapp STATIC
	libapp.cpp
)

target_include_directories(libapp
	PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(libapp
	PRIVATE precompiled
	PUBLIC libtracer
	PUBLIC lttng-ust dl
)

BuildInfo(libapp)
CleanCoverage(libapp)
Format(libapp .)
AddCppcheck(libapp)
Doxygen(libapp src/libapp)

target_compile_options(libapp
	PRIVATE ${DEFAULT_CXX_COMPILE_FLAGS}
	PRIVATE ${DEFAULT_CXX_OPTIMIZE_FLAG}
)
