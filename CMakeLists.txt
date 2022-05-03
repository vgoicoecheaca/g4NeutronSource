cmake_minimum_required(VERSION 3.8...3.18)
if (${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()
project(NeutronSource)

find_package(Geant4 REQUIRED ui_all vis_all)
find_package(ROOT REQUIRED)
include(${Geant4_USE_FILE})

include_directories(${PROJECT_SOURCE_DIR}/include
                    ${Geant4_INCLUDE_DIR}
                    ${ROOT_INCLUDE_DIRS})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)


add_executable(NeutronSource NeutronSource.cc ${sources} ${headers})
target_link_libraries(NeutronSource ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} )

set(PROJEECT_SCRIPTS
    macros/*
   )        

foreach(_script ${PROJECT_SCRIPTS})
configure_file(
  ${PROJECT_SOURCE_DIR}/${_script}
  ${PROJECT_BINARY_DIR}/${_script}
  COPYONLY
  )
endforeach()



