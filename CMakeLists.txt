# Set CMake minimum version required
cmake_minimum_required(VERSION 3.15)

# Set a variable for project name
set(PROJECT_NAME LGSim)

# Set project name
project(${PROJECT_NAME})

# Find Geant4 and use all drivers for both UI and visualization
find_package(Geant4 REQUIRED ui_all vis_all)

# Find CRY library
list(APPEND CMAKE_PREFIX_PATH "/Users/seungho/Development/CRY")
find_library(CRY_LIB CRY)
find_path(CRY_INCLUDE_DIR NAMES "CRYSetup.h" PATHS "/Users/seungho/Development/CRY/src")

# Set external libraries correctly to the install rpath for macOS
if(APPLE)
    message(STATUS "Running on macOS: -DCMAKE_INSTALL_RPATH_USE_LINK_PATH = ON")
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH true)
endif()

# Set include path for both G4 and app
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

# Locate sources and headers for compilation
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# Compile and link G4 libraries to the main app
add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cc ${sources} ${headers})
target_link_libraries(${PROJECT_NAME} ${Geant4_LIBRARIES} ${CRY_LIB})
target_include_directories(${PROJECT_NAME} PUBLIC ${CRY_INCLUDE_DIR})

# Install the executable
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
install(TARGETS ${PROJECT_NAME} DESTINATION bin)