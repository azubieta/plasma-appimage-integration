# general CPack options
set(CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR "The AppImage Project https://appimage.org")

# global options
set(CPACK_PACKAGE_CONTACT "Alexis Lopez Zubieta <contact@azubieta.net>")
#set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.md")
#set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.txt")

set(CPACK_PACKAGE_DESCRIPTION "Extensions to enhance AppImage support on plasma")

add_subdirectory(deb)

# Must be after all the package configurations
include(CPack)
