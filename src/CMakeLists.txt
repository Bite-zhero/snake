add_executable(Snake main.cpp)


set(Bin_dir "${PROJECT_SOURCE_DIR}/bin")

add_custom_target(bin ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory "${Bin_dir}"
)

set_target_properties(Snake PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/"
)


set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)

target_include_directories(Snake
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/src/
include
        ${CURSES_INCLUDE_DIRS}
)

target_link_libraries(Snake 
    PRIVATE 
        ${CURSES_LIBRARIES}
)