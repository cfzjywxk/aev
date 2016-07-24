#set(HEADER_SRC
#   ${PROJECT_SOURCE_DIR}/src
#)
##set src

LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)


#add_executable(testSampleClass sampleClass.h  testSampleClass.cpp )
add_executable(testSampleClass testSampleClass.cpp )
set_target_properties(testSampleClass PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(testSampleClass gtest gtest_main pthread libaev.a)


add_executable(test_ev_io test_ev_io.cpp )
set_target_properties(test_ev_io PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(test_ev_io gtest gtest_main pthread libaev.a)


add_executable(testLoop testLoop.cpp)
set_target_properties(testLoop PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(testLoop gtest gtest_main pthread libaev.a)

add_subdirectory(lib)
