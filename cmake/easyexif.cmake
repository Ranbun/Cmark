INCLUDE(FetchContent)

SET(EASYEXIF_GIT_URL  https://github.com/Ranbun/easyexif.git)

FetchContent_Declare(
        easyexif
        GIT_REPOSITORY    ${EASYEXIF_GIT_URL}
        GIT_TAG master
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/src/vendor/easyexif
)

FetchContent_MakeAvailable(easyexif)