file(READ "version.txt" ver)

string(REGEX MATCH "VERSION_MAJOR ([0-9]*)" _ ${ver})
set(ver_major ${CMAKE_MATCH_1})

string(REGEX MATCH "VERSION_MINOR ([0-9]*)" _ ${ver})
set(ver_minor ${CMAKE_MATCH_1})

string(REGEX MATCH "VERSION_PATCH ([0-9]*)" _ ${ver})
set(ver_patch ${CMAKE_MATCH_1})

message("IPSConfigurator version: ${ver_major}.${ver_minor}.${ver_patch}")
project(IPSConfigurator
        VERSION ${ver_major}.${ver_minor}.${ver_patch}
        HOMEPAGE_URL "http://git-holding.ru"
        LANGUAGES CXX)

set (CPACK_PACKAGE_VERSION_MAJOR ${ver_major})
set (CPACK_PACKAGE_VERSION_MINOR ${ver_minor})
set (CPACK_PACKAGE_VERSION_PATCH ${ver_patch})

add_definitions(-DVERSION_MAJOR=${ver_major})
add_definitions(-DVERSION_MINOR=${ver_minor})
add_definitions(-DVERSION_PATCH=${ver_patch})
