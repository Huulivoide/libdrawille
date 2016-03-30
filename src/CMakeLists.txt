find_library(MATH m)

set(SOURCE_FILES Canvas.c stamp.c mat3.c polygon.c point.c utils.c)

add_library(libdrawille ${SOURCE_FILES})
target_link_libraries(libdrawille ${MATH})

set_property(TARGET libdrawille PROPERTY C_STANDARD 11)
set_property(TARGET libdrawille PROPERTY C_STANDARD_REQUIRED ON)
