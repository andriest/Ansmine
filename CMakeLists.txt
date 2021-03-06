CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

PROJECT(Ansmine)

SET(VERSION_MAJOR		0)
SET(VERSION_MINOR		0)
SET(VERSION_REVISION 	1)

CONFIGURE_FILE(src/config.hpp.in src/config.hpp)

FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui QtNetwork REQUIRED)
FIND_PACKAGE(QJson REQUIRED)

IF (APPLE)
	FIND_LIBRARY(CARBON_LIBRARY Carbon)
ENDIF()

INCLUDE(${QT_USE_FILE})

MESSAGE(STATUS "qjson inc: ${QJSON_INCLUDE_DIR}")
MESSAGE(STATUS "qjson lib: ${QJSON_LIBRARY}")
MESSAGE(STATUS "qjson libs: ${QJSON_LIBRARIES}")
MESSAGE(STATUS "qt libs:" ${QT_LIBRARIES})

SET( ANSMINE_SOURCES
	src/redmine.hpp
	src/redmine_model.hpp
	src/redmine_model.cpp
	src/issue.hpp
	src/issue.cpp
	src/ansmine.hpp
	src/ansmine.cpp
	src/gethub.hpp
	src/settings_chat.hpp
	src/settings_chat.cpp
	src/separate_class.hpp
	)
	
SET( ANSMINED_SOURCES
	src/tray.hpp
	src/tray.cpp
	src/redmine.hpp
	src/login.hpp
	src/login.cpp
	src/redmine_model.hpp
	src/redmine_model.cpp
	src/issue.hpp
	src/issue.cpp
	src/separate_class.hpp
	)

INCLUDE_DIRECTORIES(
	src
	${QJSON_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	)
	

QT4_WRAP_UI(ANSMINE_UI_HEADERS 
	ui/login.ui
	ui/main.ui
	)
QT4_WRAP_CPP(ANSMINE_MOC 
	src/ansmine.hpp
	src/settings_chat.hpp
	)
QT4_WRAP_CPP(ANSMINED_MOC 
	src/tray.hpp
	src/redmine.hpp
	src/login.hpp
	)
QT4_WRAP_CPP(REDMINE_CLIENT_MOC 
	src/redmine.hpp
	)
QT4_WRAP_CPP(GETHUB_CLIENT_MOC
	src/gethub.hpp
	)
QT4_ADD_RESOURCES(ANSMINE_RESOURCES
	resources/ansmine.qrc
	)
	
ADD_LIBRARY(RedmineClient src/redmine.cpp src/redmine.hpp ${REDMINE_CLIENT_MOC})
ADD_LIBRARY(GethubClient src/gethub.cpp src/gethub.hpp ${GETHUB_CLIENT_MOC})

IF( APPLE )
	SET(PROGNAME Ansmine)
	SET(MACOSX_BUNDLE_ICON_FILE digaku-logo.icns)


	# For Ansmine Daemon

	ADD_EXECUTABLE(${PROGNAME}d MACOSX_BUNDLE src/ansmined_main.cpp 
		${ANSMINE_UI_HEADERS}
		${ANSMINED_SOURCES} 
		${ANSMINED_MOC}
		${ANSMINE_RESOURCES}
		)


	TARGET_LINK_LIBRARIES(${PROGNAME}d
		RedmineClient
		${QT_LIBRARIES}
		${QJSON_LIBRARY}
		${CARBON_LIBRARY}
		)

	ADD_CUSTOM_COMMAND( TARGET ${PROGNAME}d POST_BUILD 
		COMMAND mkdir ARGS -p ${CMAKE_CURRENT_BINARY_DIR}/${PROGNAME}.app/Contents/Frameworks
		COMMAND cp ARGS ${QJSON_LIBRARIY_DIR}/libqjson.* ${CMAKE_CURRENT_BINARY_DIR}/${PROGNAME}.app/Contents/Frameworks/
		COMMAND macdeployqt ARGS ${PROGNAME}d.app
		)

	ADD_EXECUTABLE(${PROGNAME} MACOSX_BUNDLE src/ansmine_main.cpp 
		${ANSMINE_SOURCES} 
		${ANSMINE_UI_HEADERS} 
		${ANSMINE_MOC} 
		${ANSMINE_RESOURCES}
		)
	
	TARGET_LINK_LIBRARIES(${PROGNAME} 
		RedmineClient
		GethubClient
		${QT_LIBRARIES}
		${QJSON_LIBRARY}
		${CARBON_LIBRARY}
		)
	
	ADD_DEPENDENCIES(${PROGNAME} ${PROGNAME}d)
	
	ADD_CUSTOM_COMMAND(TARGET ${PROGNAME} PRE_BUILD
		COMMAND rm ARGS -rf ${CMAKE_CURRENT_SOURCE_DIR}/${PROGNAME}.app/Contents/Frameworks
		)
	
	ADD_CUSTOM_COMMAND( TARGET ${PROGNAME} POST_BUILD 
		COMMAND mkdir ARGS -p ${CMAKE_CURRENT_BINARY_DIR}/${PROGNAME}.app/Contents/Resources
		COMMAND cp ARGS ${CMAKE_CURRENT_SOURCE_DIR}/resources/digaku-logo.icns ${CMAKE_CURRENT_BINARY_DIR}/${PROGNAME}.app/Contents/Resources	
		COMMAND mkdir ARGS -p ${CMAKE_CURRENT_BINARY_DIR}/${PROGNAME}.app/Contents/Frameworks
		COMMAND cp ARGS ${QJSON_LIBRARIY_DIR}/libqjson.* ${CMAKE_CURRENT_BINARY_DIR}/${PROGNAME}.app/Contents/Frameworks/
#		COMMAND cp ARGS ${CMAKE_CURRENT_BINARY_DIR}/${PROGNAME}d.app/Contents/MacOS/${PROGNAME}d ${CMAKE_CURRENT_BINARY_DIR}/${PROGNAME}.app/Contents/MacOS/ 
		COMMAND macdeployqt ARGS ${PROGNAME}.app
		COMMAND cp ARGS ${CMAKE_CURRENT_SOURCE_DIR}/resources/*.wav ${CMAKE_CURRENT_BINARY_DIR}/${PROGNAME}.app/Contents/Resources/
		)

		
	INSTALL(TARGETS ${PROGNAME} DESTINATION ./)
	INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/dist/osx/README.txt DESTINATION ./)

ELSE( APPLE )
	SET(PROGNAME ansmine)
	
	ADD_EXECUTABLE(${PROGNAME} src/ansmine_main.cpp 
		${ANSMINE_SOURCES} 
		${ANSMINE_UI_HEADERS} 
		${ANSMINE_MOC} 
		${ANSMINE_RESOURCES}
		)
	
	TARGET_LINK_LIBRARIES(${PROGNAME} 
		RedmineClient
		GethubClient
		${QT_LIBRARIES}
		${QJSON_LIBRARY}
		)
	
	# For Ansmine Daemon

	ADD_EXECUTABLE(${PROGNAME}d src/ansmined_main.cpp 
		${ANSMINE_UI_HEADERS}
		${ANSMINED_SOURCES} 
		${ANSMINED_MOC}
		${ANSMINE_RESOURCES}
		)

	TARGET_LINK_LIBRARIES(${PROGNAME}d
		RedmineClient
		${QT_LIBRARIES}
		${QJSON_LIBRARY}
		${CARBON_LIBRARY}
		)

ENDIF( APPLE )





INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/CDist.txt)

