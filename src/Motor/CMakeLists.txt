project(CimaEngine3v)

add_subdirectory(Render)
add_subdirectory(App)
add_subdirectory(GUI)
add_subdirectory(Primitivos)

target_sources(CimaEngine3v PRIVATE Motor.cpp)

