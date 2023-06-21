project(mapget-http-service)

add_library(mapget-http-service STATIC
  include/mapget/http-service/http-service.h
  src/http-service.cpp)

target_include_directories(mapget-http-service
  PUBLIC
    include
  PRIVATE
    include/mapget/http-service)

target_link_libraries(mapget-http-service
  PUBLIC
    httplib::httplib
    yaml-cpp
    mapget-service
    mapget-http-datasource)

if (MSVC)
  target_compile_definitions(mapget-http-service
    PRIVATE
      _CRT_SECURE_NO_WARNINGS)

  set_target_properties(mapget-http-service PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

if (WITH_COVERAGE)
  append_coverage_compiler_flags_to_target(mapget-http-service)
endif()