set(LIB_SRC
  ${LIB_SRC}
  ${CMAKE_CURRENT_SOURCE_DIR}/anfd.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ev_io.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ev_loop.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ev_watcher.cpp
  PARENT_SCOPE
)
