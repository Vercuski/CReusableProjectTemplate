add_executable(basic_usage basic_usage.c)
target_link_libraries(basic_usage PRIVATE ctemplate::ctemplate)
ctemplate_set_warnings(basic_usage ${CTEMPLATE_WARNINGS_AS_ERRORS})
