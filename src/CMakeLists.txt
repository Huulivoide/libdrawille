find_library(MATH m)

set(SOURCE_FILES Canvas.c stamp.c mat3.c polygon.c point.c utils.c x86/sse.c )
set_source_files_properties(x86/sse.c PROPERTIES COMPILE_FLAGS "-msse -msse2 -msse4")

add_library(libdrawille ${SOURCE_FILES})
target_link_libraries(libdrawille ${MATH})

