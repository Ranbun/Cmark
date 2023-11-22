INCLUDE(FetchContent)

SET(EASYEXIF_GIT_URL  https://github.com/Ranbun/easyexif.git)

FetchContent_Declare(
        easyexif
        GIT_REPOSITORY    ${EASYEXIF_GIT_URL}
        GIT_TAG master
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/src/vendor/easyexif
)

# 设置进度信息
SET(FETCH_CONTENT_STATUS "Configuring external project easyexif")
MESSAGE(STATUS ${FETCH_CONTENT_STATUS})

FetchContent_MakeAvailable(easyexif)

# 设置进度信息
SET(FETCH_CONTENT_STATUS "Finished configuring external project easyexif")
MESSAGE(STATUS ${FETCH_CONTENT_STATUS})

