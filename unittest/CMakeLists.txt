#set(HEADER_SRC
#   ${PROJECT_SOURCE_DIR}/src
#)
##set src

LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)


#add_executable(testSampleClass sampleClass.h  testSampleClass.cpp )
add_executable(testSampleClass testSampleClass.cpp )
set_target_properties(testSampleClass PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(testSampleClass gtest gtest_main pthread libaev.a)

add_subdirectory(lib)
