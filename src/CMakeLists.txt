ADD_SUBDIRECTORY(Cmark)
ADD_SUBDIRECTORY(vendor)

IF(WITH_TEST)
    ADD_SUBDIRECTORY(test)
ENDIF(WITH_TEST)
