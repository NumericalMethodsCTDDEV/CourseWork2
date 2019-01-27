include_directories(utils core)

add_subdirectory(core)
add_subdirectory(ui)
add_subdirectory(utils)

add_executable(CourseWork2 main.cpp)
target_link_libraries(CourseWork2 UI)
