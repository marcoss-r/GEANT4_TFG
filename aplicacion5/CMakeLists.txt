#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 3.16...3.27)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(prueba)



#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# See the documentation for a guide on how to enable/disable specific components
#
find_package(Geant4 REQUIRED ui_all vis_all)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, use our local headers, and link it to the Geant4 libraries
#
add_executable(aplicacion5 main.cc ${sources} ${headers})
target_include_directories(aplicacion5 PRIVATE include)
target_link_libraries(aplicacion5 PRIVATE ${Geant4_LIBRARIES})

#-----------------------------------------------------------------------------
# Copy macro files into build/mac_files for better organization
#-----------------------------------------------------------------------------

set(APLICACION5_SCRIPTS
  init_vis.mac
  init_vis2.mac
  run_batch.mac
  # run2.mac
  # vis.mac
)

foreach(_script ${APLICACION5_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/mac_files/${_script}
    ${PROJECT_BINARY_DIR}/mac_files/${_script}
    COPYONLY
  )
endforeach()

