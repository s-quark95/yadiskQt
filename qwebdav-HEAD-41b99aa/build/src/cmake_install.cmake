# Install script for directory: /home/quark/Yandex.Disk/YaDisk/qwebdav-HEAD-41b99aa/src

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/home/quark/Yandex.Disk/YaDisk/qwebdav-HEAD-41b99aa/build/_preferred_path_")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "RelWithDebInfo")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "0")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Devel")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/quark/Yandex.Disk/YaDisk/qwebdav-HEAD-41b99aa/build/_preferred_path_/include/qwebdav/webdav.h;/home/quark/Yandex.Disk/YaDisk/qwebdav-HEAD-41b99aa/build/_preferred_path_/include/qwebdav/webdav_url_info.h;/home/quark/Yandex.Disk/YaDisk/qwebdav-HEAD-41b99aa/build/_preferred_path_/include/qwebdav/webdav_export.h;/home/quark/Yandex.Disk/YaDisk/qwebdav-HEAD-41b99aa/build/_preferred_path_/include/qwebdav/QWebdav;/home/quark/Yandex.Disk/YaDisk/qwebdav-HEAD-41b99aa/build/_preferred_path_/include/qwebdav/QWebdavUrlInfos")
  IF (CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  ENDIF (CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
  IF (CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  ENDIF (CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
FILE(INSTALL DESTINATION "/home/quark/Yandex.Disk/YaDisk/qwebdav-HEAD-41b99aa/build/_preferred_path_/include/qwebdav" TYPE FILE FILES
    "/home/quark/Yandex.Disk/YaDisk/qwebdav-HEAD-41b99aa/src/webdav.h"
    "/home/quark/Yandex.Disk/YaDisk/qwebdav-HEAD-41b99aa/src/webdav_url_info.h"
    "/home/quark/Yandex.Disk/YaDisk/qwebdav-HEAD-41b99aa/src/webdav_export.h"
    "/home/quark/Yandex.Disk/YaDisk/qwebdav-HEAD-41b99aa/src/QWebdav"
    "/home/quark/Yandex.Disk/YaDisk/qwebdav-HEAD-41b99aa/src/QWebdavUrlInfos"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Devel")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FOREACH(file
      "$ENV{DESTDIR}/home/quark/Yandex.Disk/YaDisk/qwebdav-HEAD-41b99aa/build/_preferred_path_/lib/libqwebdav.so.0.0.1"
      "$ENV{DESTDIR}/home/quark/Yandex.Disk/YaDisk/qwebdav-HEAD-41b99aa/build/_preferred_path_/lib/libqwebdav.so.0"
      "$ENV{DESTDIR}/home/quark/Yandex.Disk/YaDisk/qwebdav-HEAD-41b99aa/build/_preferred_path_/lib/libqwebdav.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      FILE(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    ENDIF()
  ENDFOREACH()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/quark/Yandex.Disk/YaDisk/qwebdav-HEAD-41b99aa/build/_preferred_path_/lib/libqwebdav.so.0.0.1;/home/quark/Yandex.Disk/YaDisk/qwebdav-HEAD-41b99aa/build/_preferred_path_/lib/libqwebdav.so.0;/home/quark/Yandex.Disk/YaDisk/qwebdav-HEAD-41b99aa/build/_preferred_path_/lib/libqwebdav.so")
  IF (CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  ENDIF (CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
  IF (CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  ENDIF (CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
FILE(INSTALL DESTINATION "/home/quark/Yandex.Disk/YaDisk/qwebdav-HEAD-41b99aa/build/_preferred_path_/lib" TYPE SHARED_LIBRARY FILES
    "/home/quark/Yandex.Disk/YaDisk/qwebdav-HEAD-41b99aa/build/lib/libqwebdav.so.0.0.1"
    "/home/quark/Yandex.Disk/YaDisk/qwebdav-HEAD-41b99aa/build/lib/libqwebdav.so.0"
    "/home/quark/Yandex.Disk/YaDisk/qwebdav-HEAD-41b99aa/build/lib/libqwebdav.so"
    )
  FOREACH(file
      "$ENV{DESTDIR}/home/quark/Yandex.Disk/YaDisk/qwebdav-HEAD-41b99aa/build/_preferred_path_/lib/libqwebdav.so.0.0.1"
      "$ENV{DESTDIR}/home/quark/Yandex.Disk/YaDisk/qwebdav-HEAD-41b99aa/build/_preferred_path_/lib/libqwebdav.so.0"
      "$ENV{DESTDIR}/home/quark/Yandex.Disk/YaDisk/qwebdav-HEAD-41b99aa/build/_preferred_path_/lib/libqwebdav.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      IF(CMAKE_INSTALL_DO_STRIP)
        EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "${file}")
      ENDIF(CMAKE_INSTALL_DO_STRIP)
    ENDIF()
  ENDFOREACH()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

