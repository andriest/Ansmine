
FIND_PATH(QJSON_INCLUDE_DIR NAME parser.h PATHS
	/usr/include/qjson
	/usr/local/include/qjson
	/opt/include/qjson
	/opt/local/include/qjson
	)

FIND_FILE(QJSON_LIBRARY NAMES libqjson.dylib libqjson.0.dylib libqjson.so PATHS
	/opt/lib
	/opt/local/lib
	/usr/lib
	/usr/local/lib
	)

IF(NOT QJSON_INCLUDE_DIR OR NOT QJSON_LIBRARY)
	message(FATAL_ERROR "Cannot find qjson lib.")
ENDIF()

GET_FILENAME_COMPONENT(QJSON_LIBRARIY_DIR ${QJSON_LIBRARY} PATH)
SET(QJSON_LIBRARIES 
	${QJSON_LIBRARIY_DIR}/libqjson.dylib
	${QJSON_LIBRARIY_DIR}/libqjson.0.dylib
	)

