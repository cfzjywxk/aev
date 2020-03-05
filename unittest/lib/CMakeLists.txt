## test_tblog
set(TEST_TBLOG_SRC test_tblog.cpp)

add_executable(test_tblog ${TEST_TBLOG_SRC})

LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/build/src)
target_link_libraries(test_tblog libaev.a pthread)
