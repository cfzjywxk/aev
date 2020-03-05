### for unit tests

LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/build/src)

add_executable(test_sampleclass test_sampleclass.cpp )
set_target_properties(test_sampleclass PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(test_sampleclass gtest gtest_main pthread libaev.a)

add_executable(test_ev_io test_ev_io.cpp )
set_target_properties(test_ev_io PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(test_ev_io gtest gtest_main pthread libaev.a)

add_executable(test_loop test_loop.cpp)
set_target_properties(test_loop PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(test_loop gtest gtest_main pthread libaev.a)

add_subdirectory(lib)
