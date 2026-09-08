add_executable(ctemplate_tests test_ctemplate.c)
target_link_libraries(ctemplate_tests PRIVATE ctemplate::ctemplate)
target_include_directories(ctemplate_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
ctemplate_set_warnings(ctemplate_tests ${CTEMPLATE_WARNINGS_AS_ERRORS})

add_test(NAME ctemplate_tests COMMAND ctemplate_tests)
