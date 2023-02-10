##################################################    Project     ##################################################
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project               (AdvanceCpp VERSION 1.0 LANGUAGES CXX C)
list                  (APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set_property          (GLOBAL PROPERTY USE_FOLDERS ON)
set                   (CMAKE_CXX_STANDARD 17)
include               (set_max_warning_level)
set_max_warning_level ()

##################################################    Options     ##################################################
option(BUILD_TESTS "Build tests." OFF)
##################################################    Sources     ##################################################
file(GLOB_RECURSE PROJECT_CMAKE_UTILS cmake/*.cmake)
file(GLOB_RECURSE PROJECT_MISC *.md *.txt)
#set(CMAKE_TOOLCHAIN_FILE "C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake")
macro(add_folder ARG)
  message("Adding folder ${ARG}")
  file(GLOB_RECURSE PROJECT_${ARG}_HEADERS libs/${ARG}/inc/*.h libs/${ARG}/inc/*.hpp)
  file(GLOB_RECURSE PROJECT_${ARG}_SOURCES libs/${ARG}/src/*.c libs/${ARG}/src/*.cpp)
  list(APPEND PROJECT_HEADERS ${PROJECT_${ARG}_HEADERS})
  list(APPEND PROJECT_SOURCES ${PROJECT_${ARG}_SOURCES})
endmacro()

add_folder(core)
add_folder(network)

set (PROJECT_FILES 
  ${PROJECT_HEADERS} 
  ${PROJECT_SOURCES} 
  ${PROJECT_CMAKE_UTILS} 
  ${PROJECT_MISC})

file(GLOB_RECURSE PROJECT_APPLICATION_HEADERS inc/*.h inc/*.hpp)
file(GLOB_RECURSE PROJECT_APPLICATION_SOURCES src/*.c src/*.cpp)

include            (assign_source_group)
assign_source_group(${PROJECT_FILES})

##################################################  Dependencies  ##################################################
include(import_library)

macro( config_project LIBNAME )    
  list (APPEND PROJECT_INCLUDE_DIRS ${${LIBNAME}_INCLUDE_DIR})
  list (APPEND PROJECT_LIBRARIES ${${LIBNAME}_LIBRARY})  
endmacro()

# Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(QT_DIR "D:/Qt/5.15.2/msvc2019_64//lib/")
list(APPEND CMAKE_PREFIX_PATH "D:\\Qt\\5.15.2\\msvc2019_64\\lib\\cmake\\")
find_package(Qt5 REQUIRED COMPONENTS Widgets Network WebSockets)
list(APPEND PROJECT_LIBRARIES Qt5::Widgets Qt5::Network Qt5::WebSockets Qt5::Core)

# GoogleTest
find_package(GTest CONFIG REQUIRED)
list(APPEND PROJECT_LIBRARIES  GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)

# # ImGUI
# find_package(imgui CONFIG REQUIRED)
# list(APPEND PROJECT_LIBRARIES imgui::imgui)
# list(APPEND PROJECT_INCLUDE_DIRS ${IMGUI_INCLUDE_DIRS})

# # OpenGL
# find_package(GLUT REQUIRED)
# list(APPEND PROJECT_LIBRARIES GLUT::GLUT)

# find_package(glfw3 CONFIG REQUIRED)
# list(APPEND PROJECT_LIBRARIES glfw)

# find_package(GLEW CONFIG REQUIRED)
# list(APPEND PROJECT_LIBRARIES GLEW::GLEW)

# # fmt
# find_package(fmt CONFIG REQUIRED)
# list(APPEND PROJECT_LIBRARIES fmt::fmt)

# spdlog
find_package(spdlog CONFIG REQUIRED)
list(APPEND PROJECT_LIBRARIES spdlog::spdlog spdlog::spdlog_header_only)

# GSTREAMER
find_library(GLIB NAMES glib-2.0 HINTS "C:/gstreamer/1.0/msvc_x86_64/lib/" REQUIRED)
find_library(GSTREAMER NAMES gstreamer-1.0 HINTS "C:/gstreamer/1.0/msvc_x86_64/lib/" REQUIRED)
find_library(GOBJECT NAMES gobject-2.0 HINTS "C:/gstreamer/1.0/msvc_x86_64/lib/" REQUIRED)
find_library(GTHREADS NAMES gthread-2.0 HINTS "C:/gstreamer/1.0/msvc_x86_64/lib/" REQUIRED)
find_library(GSTREAMER-VIDEO NAMES gstvideo-1.0 HINTS "C:/gstreamer/1.0/msvc_x86_64/lib/" REQUIRED)
list(APPEND PROJECT_INCLUDE_DIRS "C:/gstreamer/1.0/msvc_x86_64/include/")
list(APPEND PROJECT_INCLUDE_DIRS "C:/gstreamer/1.0/msvc_x86_64/include/glib-2.0/")
list(APPEND PROJECT_INCLUDE_DIRS "C:/gstreamer/1.0/msvc_x86_64/include/gstreamer-1.0/")
list(APPEND PROJECT_INCLUDE_DIRS "C:/gstreamer/1.0/msvc_x86_64/lib/glib-2.0/include/")
list(APPEND PROJECT_LIBRARIES ${GLIB} ${GSTREAMER} ${GOBJECT} ${GTHREAD} ${GSTREAMER-VIDEO})

##################################################    Targets     ##################################################

add_library(${PROJECT_NAME}_lib ${PROJECT_FILES})
set_target_properties(${PROJECT_NAME}_lib PROPERTIES ENABLE_EXPORTS ON)
target_include_directories(${PROJECT_NAME}_lib PUBLIC 
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libs/>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<INSTALL_INTERFACE:include> PRIVATE libs)
target_include_directories(${PROJECT_NAME}_lib PUBLIC ${PROJECT_INCLUDE_DIRS})
target_link_libraries     (${PROJECT_NAME}_lib PUBLIC ${PROJECT_LIBRARIES} ${GTHREAD})
target_compile_definitions(${PROJECT_NAME}_lib PUBLIC ${PROJECT_COMPILE_DEFINITIONS})
set_target_properties     (${PROJECT_NAME}_lib PROPERTIES LINKER_LANGUAGE CXX)

if(NOT BUILD_SHARED_LIBS)
  string               (TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)
  set_target_properties(${PROJECT_NAME}_lib PROPERTIES COMPILE_FLAGS -D${PROJECT_NAME_UPPER}_STATIC)
endif()

add_executable(${PROJECT_NAME} ${PROJECT_APPLICATION_HEADERS} ${PROJECT_APPLICATION_SOURCES})
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/")

target_include_directories(${PROJECT_NAME} PUBLIC 
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libs/>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<INSTALL_INTERFACE:include> PRIVATE libs inc)
target_link_libraries     (${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_lib ${GTHREAD})
target_include_directories(${PROJECT_NAME} PUBLIC )

##################################################    Testing     ##################################################
if(BUILD_TESTS)
  enable_testing       ()
  set                  (TEST_MAIN_NAME test_main)
  file                 (GLOB TEST_MAIN_SOURCES tests/internal/*.cpp)
  add_executable       (${TEST_MAIN_NAME} ${TEST_MAIN_SOURCES})
  target_link_libraries(${TEST_MAIN_NAME} ${PROJECT_NAME}_lib ${PROJECT_LIBRARIES})
  set_property         (TARGET ${TEST_MAIN_NAME} PROPERTY FOLDER tests)
  assign_source_group  (${TEST_MAIN_SOURCES})

  file(GLOB PROJECT_TEST_CPPS tests/*.cpp)
  foreach(_SOURCE ${PROJECT_TEST_CPPS})
    get_filename_component    (_NAME ${_SOURCE} NAME_WE)
    add_executable            (${_NAME} ${_SOURCE} $<TARGET_OBJECTS:${TEST_MAIN_NAME}>)
    target_include_directories(${_NAME} PUBLIC 
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
      $<INSTALL_INTERFACE:include> PRIVATE source)
    target_include_directories(${_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS})
    target_link_libraries     (${_NAME} PUBLIC ${PROJECT_LIBRARIES})
    target_compile_definitions(${_NAME} PUBLIC ${PROJECT_COMPILE_DEFINITIONS})
    add_test                  (${_NAME} ${_NAME})
    set_property              (TARGET ${_NAME} PROPERTY FOLDER tests)
    assign_source_group       (${_SOURCE})
  endforeach()
endif()

##################################################  Installation  ##################################################
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-config
  RUNTIME DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)
install(EXPORT  ${PROJECT_NAME}-config DESTINATION cmake)
export (TARGETS ${PROJECT_NAME}        FILE        ${PROJECT_NAME}-config.cmake)
