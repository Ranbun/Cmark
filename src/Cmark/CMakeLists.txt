SET(target_ Cmark)

FILE(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h
        ${third_party}/easyexif/exif.cpp ${third_party}/easyexif/exif.h
        ${third_party}/sceneLayout/SceneLayoutEditor.cpp ${third_party}/sceneLayout/SceneLayoutEditor.h
        ${third_party}/sceneLayout/SceneLayoutSettings.cpp ${third_party}/sceneLayout/SceneLayoutSettings.h
        ${third_party}/QtExtened/qexifimageheader.cpp
        ${third_party}/sceneLayout/dialog.ui
)

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

FIND_PACKAGE(Qt5Widgets CONFIG REQUIRED)
FIND_PACKAGE(Qt5Gui CONFIG REQUIRED)

SET(CMARK_ICON ${CMAKE_SOURCE_DIR}/sources/executableLogo/Cmark.rc)

ADD_EXECUTABLE(${target_} ${SOURCE_FILES} ${CMARK_ICON})

TARGET_INCLUDE_DIRECTORIES(${target_} PRIVATE
        ${third_party}/easyexif
        ${third_party}/sceneLayout
        ${third_party}/QtExtened
        ${CMAKE_CURRENT_SOURCE_DIR})

SET(LIBRARIES Qt5::Widgets Qt5::Gui)

IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
    LIST(APPEND LIBRARIES pthread)
ENDIF()

TARGET_LINK_LIBRARIES(${target_} PRIVATE ${LIBRARIES})

SET(EXE_OUTPUT_DIR ${PROJECT_BINARY_DIR}/bin)

FILE(INSTALL ${CMAKE_SOURCE_DIR}/sources/executableLogo DESTINATION ${EXE_OUTPUT_DIR}/sources/)
FILE(INSTALL ${CMAKE_SOURCE_DIR}/sources/icons DESTINATION ${EXE_OUTPUT_DIR}/sources/)
FILE(INSTALL ${CMAKE_SOURCE_DIR}/sources/logos DESTINATION ${EXE_OUTPUT_DIR}/sources/)

SET_TARGET_PROPERTIES(
        ${target_} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${EXE_OUTPUT_DIR}"
        # LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin"
        # ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib"
)
