cmake_minimum_required(VERSION 3.10)
project(RTKLIB VERSION 2.4.2 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 99)

# Define source files
set(RTKLIB_SOURCES
    src/utils.c
    src/convkml.c
    src/convrnx.c
    src/datum.c
    src/download.c
    src/ephemeris.c
    src/geoid.c
    src/ionex.c
    src/lambda.c
    src/options.c
    src/pntpos.c
    src/postpos.c
    src/ppp.c
    src/ppp_ar.c
    src/preceph.c
    src/qzslex.c
    src/rcvraw.c
    src/rinex.c
    src/rtcm.c
    src/rtcm2.c
    src/rtcm3.c
    src/rtcm3e.c
    src/rtkcmn.c
    src/rtklib.h
    src/rtkpos.c
    src/rtksvr.c
    src/sbas.c
    src/solution.c
    src/stream.c
    src/streamsvr.c
    src/tle.c
    src/rcv/binex.c
    src/rcv/crescent.c
    src/rcv/gw10.c
    src/rcv/javad.c
    src/rcv/novatel.c
    src/rcv/nvs.c
    src/rcv/rcvlex.c
    src/rcv/rt17.c
    src/rcv/septentrio.c
    src/rcv/skytraq.c
    src/rcv/ss2.c
    src/rcv/ublox.c
)

# Build RTKLIB as a shared library
add_library(rtklib SHARED ${RTKLIB_SOURCES})

# Build RTKLIB as a static library
add_library(rtklib_static STATIC ${RTKLIB_SOURCES})
set_target_properties(rtklib_static PROPERTIES OUTPUT_NAME rtklib)

# Add include directories
target_include_directories(rtklib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
)
target_include_directories(rtklib_static PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
)

# Add platform-specific libraries
if(WIN32)
    if(MSVC)
        add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    endif()
    
    target_link_libraries(rtklib PRIVATE winmm ws2_32)
    target_link_libraries(rtklib_static PRIVATE winmm ws2_32)
else()
    add_compile_options(
        -Wno-unused-variable
        -Wno-conversion
        -Wno-sign-conversion
        -Wno-deprecated-declarations
        -Wno-format
    )
    
    target_link_libraries(rtklib PRIVATE m pthread)
    target_link_libraries(rtklib_static PRIVATE m pthread)
endif()

# Define compile options for navigation systems
option(ENABLE_GPS "Enable GPS" ON)
option(ENABLE_GLONASS "Enable GLONASS" ON)
option(ENABLE_GALILEO "Enable Galileo" ON)
option(ENABLE_QZSS "Enable QZSS" ON)
option(ENABLE_BEIDOU "Enable BeiDou" ON)
option(ENABLE_SBAS "Enable SBAS" ON)
option(ENABLE_IRNSS "Enable IRNSS" OFF)

# Add definition flags based on options
if(ENABLE_GLONASS)
    target_compile_definitions(rtklib PUBLIC ENAGLO)
    target_compile_definitions(rtklib_static PUBLIC ENAGLO)
endif()
if(ENABLE_GALILEO)
    target_compile_definitions(rtklib PUBLIC ENAGAL)
    target_compile_definitions(rtklib_static PUBLIC ENAGAL)
endif()
if(ENABLE_QZSS)
    target_compile_definitions(rtklib PUBLIC ENAQZS)
    target_compile_definitions(rtklib_static PUBLIC ENAQZS)
endif()
if(ENABLE_BEIDOU)
    target_compile_definitions(rtklib PUBLIC ENACMP)
    target_compile_definitions(rtklib_static PUBLIC ENACMP)
endif()

# Install targets
install(TARGETS rtklib rtklib_static
    EXPORT RTKLIBTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

# Install headers
install(FILES src/rtklib.h DESTINATION include)

# Export targets for use by other projects
install(EXPORT RTKLIBTargets
    FILE RTKLIBTargets.cmake
    NAMESPACE RTKLIB::
    DESTINATION lib/cmake/RTKLIB
)

# Create and install package configuration file
include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/RTKLIBConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/RTKLIBConfig.cmake
    INSTALL_DESTINATION lib/cmake/RTKLIB
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/RTKLIBConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/RTKLIBConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/RTKLIBConfigVersion.cmake
    DESTINATION lib/cmake/RTKLIB
)

# Create a simple RTKLIBConfig.cmake.in file in the cmake directory:
# File: cmake/RTKLIBConfig.cmake.in
file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/RTKLIBConfig.cmake.in
"@PACKAGE_INIT@

include(\${CMAKE_CURRENT_LIST_DIR}/RTKLIBTargets.cmake)
check_required_components(RTKLIB)
")
