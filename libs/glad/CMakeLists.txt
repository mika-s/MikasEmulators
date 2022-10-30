set(SOURCES_GLAD
        include/glad/glad.h
        include/KHR/khrplatform.h
        src/glad.c
        )

add_library(Glad STATIC ${SOURCES_GLAD})
target_include_directories(Glad PUBLIC include)
