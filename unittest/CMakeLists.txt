## for unit tests

## debug function
#function(dump_cmake_variables)
#    get_cmake_property(_variableNames VARIABLES)
#    list (SORT _variableNames)
#    foreach (_variableName ${_variableNames})
#        if (ARGV0)
#            unset(MATCHED)
#            string(REGEX MATCH ${ARGV0} MATCHED ${_variableName})
#            if (NOT MATCHED)
#                continue()
#            endif()
#        endif()
#        message(STATUS "${_variableName}=${${_variableName}}")
#    endforeach()
#endfunction()
#dump_cmake_variables()
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/src)

add_executable(test_sampleclass test_sampleclass.cpp )
add_dependencies (test_sampleclass aev_static)
set_target_properties(test_sampleclass PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(test_sampleclass gtest gtest_main pthread aev)

add_executable(test_ev_io test_ev_io.cpp )
add_dependencies (test_ev_io aev_static)
set_target_properties(test_ev_io PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(test_ev_io gtest gtest_main pthread aev)

add_executable(test_loop test_loop.cpp)
add_dependencies (test_loop aev_static)
set_target_properties(test_loop PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(test_loop gtest gtest_main pthread aev)

add_subdirectory(lib)
