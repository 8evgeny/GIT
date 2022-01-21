if((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    add_compile_options(
        -Werror

        -Wall
        -Wextra
        -Wpedantic

        -Wcast-align
        -Wcast-qual
        -Wconversion
        -Wctor-dtor-privacy
        -Wenum-compare
        -Wfloat-equal
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Woverloaded-virtual
        -Wredundant-decls
        -Wsign-conversion
        -Wsign-promo
      #  -Weffc++
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
   #add_compile_options(/W4 /WX)
#   add_compile_options(/FA)
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_PATH}")
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_PATH}")
    SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_PATH}")
    SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_PATH}")
    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_PATH}")
    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_PATH}")
endif()

if(NOT CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

# Adding clang-tidy target if executable is found
#find_program(CLANG_TIDY "clang-tidy")
#if(CLANG_TIDY)
#    add_custom_target(
#        clang-tidy
#        COMMAND /usr/bin/clang-tidy
#        -config=''
#)
#set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-clang-diagnostic-*,clang-analyzer-*,-*)
#set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,bugprone-*)
#endif()
