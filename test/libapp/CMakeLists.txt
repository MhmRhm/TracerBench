add_executable(test_libapp
	libapp_tests.cpp
)

target_link_libraries(test_libapp
	PRIVATE precompiled
	PRIVATE libapp
)

Format(test_libapp .)
AddTests(test_libapp)
EnableCoverage(libapp)
