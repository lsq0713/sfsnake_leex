cmake_minimum_required(VERSION 3.16)

project(sfSnakePlus)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

find_package(SFML 2.5 COMPONENTS
  system window graphics audio REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(main ${SOURCES})
target_include_directories(main PUBLIC src thirdparty/SFML/include)
target_link_libraries(main PRIVATE sfml-system sfml-window sfml-graphics sfml-audio)
set_property(TARGET main PROPERTY
  CXX_STANDARD          17
  CXX_STANDARD_REQUIRED ON)

file(COPY "assets/Fonts" DESTINATION "${CMAKE_BINARY_DIR}/${NAME}")
file(COPY "assets/Music" DESTINATION "${CMAKE_BINARY_DIR}/${NAME}")
file(COPY "assets/Sounds" DESTINATION "${CMAKE_BINARY_DIR}/${NAME}")
file(COPY "assets/Image" DESTINATION "${CMAKE_BINARY_DIR}/${NAME}")
