add_library(ode SHARED)

file(GLOB_RECURSE SOURCE_HPP  "src/*.h")
file(GLOB_RECURSE SOURCE_CPP  "src/*.cpp")

target_sources(ode PRIVATE
    ${SOURCE_HPP}
    ${SOURCE_CPP}
)

target_compile_definitions(ode
    PUBLIC
    dSINGLE
    dNODEBUG
    __cdecl=
)

if (WIN32)
    target_compile_definitions(ode PRIVATE -DWIN32=1)
endif()

target_include_directories(ode
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)