SET(target_ Cmark)

FILE(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h
        ${third_party}/easyexif/exif.cpp ${third_party}/easyexif/exif.h
)

ADD_EXECUTABLE(${target_} ${SOURCE_FILES})

TARGET_INCLUDE_DIRECTORIES(${target_} PRIVATE ${third_party}/easyexif)
TARGET_LINK_LIBRARIES(${target_} PRIVATE Qt5::Widgets)

FILE(INSTALL ${CMAKE_CURRENT_SOURCE_DIR}/sources DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
