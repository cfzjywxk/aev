### test_tblog
set(TEST_TBLOG_SRC test_tblog.cpp)

add_executable(test_tblog ${TEST_TBLOG_SRC})

LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/src)
add_dependencies (test_tblog aev_static)
target_link_libraries(test_tblog aev pthread)
