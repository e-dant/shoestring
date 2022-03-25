########################################################################
# ~~> Representation
## ~> basic project descriptions and options:
### - project name  - - - - - - any string
### - target name - - - - - - - any string
### - target type - - - - - - - one of: "executable", "static", "shared"
### - description - - - - - - - any string
### - version - - - - - - - - - major.minor.patch
### - dir_base  - - - - - - - - any path; the base path of the project
### - dir_src - - - - - - - - - any path; where to look for source files
### - dir_include - - - - - - - any path; where to look for included files
### - dir_build - - - - - - - - any path; where to place targets after building
### - source files  - - - - - - path(s), seperated by semicolons (;), to source files
### - project type  - - - - - - must be "static library", "shared library", or "executable"
### - language  - - - - - - - - one of: CXX/C/OBJC/OBJCXX/CUDA, but, really, C or CXX
### - standard  - - - - - - - - format YY; the standard's release year, such as 20
### - extensions  - - - - - - - one of: "ON", "OFF"; toggles language extensions (like -std=gnu++20)
### - compiler features - - - - this is useful for specifying 
### - compiler options  - - - - any valid gcc or clang compiler options
### - cmake minimum version - - the minimum version of cmake we accept. 3.2 is good.
### - log level - - - - - - - - one of: "off", "low", "high"; whether to or at what level to log.

# ~> project
set(project_name 
  "softserve")
set(project_description 
  "${project_name}")
set(project_target_name 
  "${project_name}")
set(project_target_type 
  "executable")
set(project_version 
  "1.0.0")
set(project_language 
  "CXX")
set(project_language_standard_yy 
  "20")
set(project_language_standard_required 
  "ON")
set(project_language_enable_extensions 
  "ON")
set(project_language_options 
  "-O3")
option(project_enable_lto 
  ON)
option(project_perform_install 
  OFF)

# ~> paths
set(dir_base 
  "..")
set(dir_src 
  "${dir_base}/src")
set(dir_build 
  "${dir_base}/build")
set(dir_include 
  "${dir_base}/${dir_src}/include")
set(dir_external 
  "${dir_base}/${dir_config}/external")
set(files_src 
  "${dir_src}/main.cpp"
)
#  "${dir_src}/server.hpp")

# ~> etc
set(cmake_minimum_version 
  "3.18")
set(log_level 
  "high")

########################################################################
# ~~> Intermediate Translation

## ~> It's good to define this early
## While we could probably get away with fairly low versions
## of CMake, it's important to encourage an explicit version,
## ideally on which is fairly recent.
## The project() command is an intermediate (and required) 
## CMake command which defines a bunch of handy stuff for us
cmake_minimum_required(VERSION ${cmake_minimum_version} FATAL_ERROR)
project(
  ${project_name}
  VERSION ${project_version}
  LANGUAGES ${project_language}
  DESCRIPTION ${project_description})

if(${project_name} STREQUAL ${PROJECT_NAME})
  ## ~> Handy variables to have for later
  string(TOUPPER ${project_language} project_language_uppercase)
  string(TOLOWER ${project_language} project_language_lowercase)

  set(project_language_standard_string 
    "${project_language_lowercase}_std_${project_language_standard_yy}")
  set(CMAKE_${project_language_uppercase}_STANDARD 
    "${project_language_standard_yy}")

  ## ~> Compiler options and features
  set(CMAKE_${project_language}_STANDARD 
    "${project_language_standard_yy}")
  set(CMAKE_${project_language_uppercase}_STANDARD_REQUIRED 
    "${project_language_standard_required}")
  set(CMAKE_${project_language_uppercase}_EXTENSIONS 
    "${project_language_extensions}")
  set(CMAKE_BUILD_TYPE 
    Release)

  ## ~> LTO
  if(project_enable_lto)
    cmake_policy(SET CMP0069 NEW)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT IPO_SUPPORT)
    if (IPO_SUPPORT)
        message(STATUS "${PROJECT_NAME}: LTO enabled")
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif()
  endif()
endif()

########################################################################
# ~~> Implementation

## Tells cmake where to:
### - look for source files, and 
### - where to place the binaries and libraries after building
set(CMAKE_SOURCE_DIR ${dir_src})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${dir_build})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${dir_build})

## ~> preliminary compiler options
#add_compile_options(${project_language_options})

## Tell cmake in what form to build this project,
## i.e. one of:
## - "shared": as a shared library, 
## - "static": a static library, 
## - "executable": or a standalone executable
if(${project_target_type} STREQUAL 
  "shared")
  add_library(
    ${project_target_name} SHARED 
      ${files_src}
  )
  set_target_properties(
    ${project_target_name} PROPERTIES VERSION 
      ${PROJECT_VERSION}
  )
  set_target_properties(
    ${project_target_name} PROPERTIES SOVERSION 
      ${PROJECT_VERSION_MAJOR}
  )

elseif(${project_target_type} STREQUAL 
  "static")
  add_library(
    ${project_target_name} 
    ${files_src}
  )

elseif(${project_target_type} STREQUAL 
  "executable")
  add_executable(
    ${project_target_name} 
    ${files_src}
  )
endif()

## ~> add project include paths
## allows "#include <...>" outside of the 
## environment's system library path
target_include_directories("${project_target_name}" 
  PRIVATE "${dir_include}/"
  PRIVATE "${dir_include}"

  PRIVATE "${dir_src}/include/"
  PRIVATE "${dir_src}/include"

  #PRIVATE "${dir_src}/"
  #PRIVATE "${dir_src}"

  PRIVATE "${dir_include}/softserve/"
  PRIVATE "${dir_include}/softserve"
)

## ~> find libraries
find_package(
  Boost REQUIRED 
  COMPONENTS
    headers
    program_options
)
find_package(
  Threads REQUIRED
)

## ~> now link them
target_link_libraries (
  ${project_target_name} PRIVATE 
    Boost::headers 
    Boost::program_options
    Threads::Threads
)

## ~> add project compiler options
target_compile_options(
  ${project_target_name} PRIVATE
  ${project_language_options}
)

##
##
## TODO FOR CONTAINER
##
##
if(project_perform_install)
  # below is for containers
  #include(GNUInstallDirs)
  #install(TARGETS cloud_run_hello RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

