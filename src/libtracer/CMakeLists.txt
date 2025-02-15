add_library(libtracer SHARED
	trace_points.cpp
	call_tracer.cpp
)

target_compile_options(libtracer
	PRIVATE ${DEFAULT_CXX_COMPILE_FLAGS}
	PRIVATE ${DEFAULT_CXX_OPTIMIZE_FLAG}
)

target_include_directories(libtracer
	PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
	PUBLIC "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

set_target_properties(libtracer PROPERTIES
	PUBLIC_HEADER src/libtracer/include/libtracer/trace_point.h
	POSITION_INDEPENDENT_CODE 1
)

set_target_properties(libtracer PROPERTIES
	PREFIX "lib"
	OUTPUT_NAME "tracer$<$<CONFIG:Debug>:d>"
)

BuildInfo(libtracer)
CleanCoverage(libtracer)
Format(libtracer .)
AddCppcheck(libtracer)
Doxygen(libtracer src/libtracer)
