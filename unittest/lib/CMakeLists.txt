## test_tblog
set(TEST_TBLOG_SRC test_tblog.cpp)

add_executable(test_tblog ${TEST_TBLOG_SRC})

target_link_libraries(test_tblog libaev.a)
#LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)
