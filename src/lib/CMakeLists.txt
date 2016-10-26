add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/malloc")

set(LIB_SRC
  ${LIB_SRC}
  ${CMAKE_CURRENT_SOURCE_DIR}/tblog.cpp
  PARENT_SCOPE
)
