cmake_minimum_required(VERSION 3.12.0)

project(leet-server LANGUAGES CXX)

#
# User options
#
option(LEET_BUILD_TESTS "Build tests" OFF)

#
# Global paths
#
set(LEET_ROOT_DIR "${PROJECT_SOURCE_DIR}")
set(LEET_LIBS_DIR "${LEET_ROOT_DIR}/external")
set(LEET_LIB_FMT_DIR "${LEET_LIBS_DIR}/fmt")
set(LEET_LIB_GSL_DIR "${LEET_LIBS_DIR}/gsl-lite")
set(LEET_LIB_JSON_DIR "${LEET_LIBS_DIR}/json")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#
# add source files to the project
#
set(LEET_SOURCE_MAIN "sources/main.cpp")

set(LEET_SOURCES_BASE
    "sources/buffer/dynamicbuffer.cpp"
    "sources/buffer/staticbuffer.cpp"
    "sources/buffer/view.cpp"
    "sources/channel/channel.cpp"
    "sources/channel/channelgroup.cpp"
    "sources/channel/channelstorage.cpp"
    "sources/channel/shared.cpp"
    "sources/cso2/buymenu.cpp"
    "sources/cso2/cosmetics.cpp"
    "sources/cso2/inventory.cpp"
    "sources/cso2/loadout.cpp"
    "sources/cso2/user.cpp"
    "sources/handlers/achievementhandler.cpp"
    "sources/handlers/chathandler.cpp"
    "sources/handlers/channelhandler.cpp"
    "sources/handlers/favoritehandler.cpp"
    "sources/handlers/hosthandler.cpp"
    "sources/handlers/loginhandler.cpp"
    "sources/handlers/optionhandler.cpp"
    "sources/handlers/roomhandler.cpp"
    "sources/handlers/userprofilehandler.cpp"
    "sources/handlers/versionhandler.cpp"
    "sources/holepunch/inpacket/forward.cpp"
    "sources/holepunch/inpacket/punch.cpp"
    "sources/holepunch/inpacket.cpp"
    "sources/holepunch/outpacket.cpp"
    "sources/packets/in/achievement/campaign.cpp"
    "sources/packets/in/favorite/setcosmetics.cpp"
    "sources/packets/in/favorite/setloadout.cpp"
    "sources/packets/in/host/changeteam.cpp"
    "sources/packets/in/host/itemused.cpp"
    "sources/packets/in/host/onplayerscored.cpp"
    "sources/packets/in/host/onplayerbuyitem.cpp"
    "sources/packets/in/host/onplayerkilled.cpp"
    "sources/packets/in/host/onplayerspawn.cpp"
    "sources/packets/in/host/onroundend.cpp"
    "sources/packets/in/host/setinvobj.cpp"
    "sources/packets/in/option/setbuymenu.cpp"
    "sources/packets/in/room/countdown.cpp"
    "sources/packets/in/room/joinrequest.cpp"
    "sources/packets/in/room/newroom.cpp"
    "sources/packets/in/room/setteam.cpp"
    "sources/packets/in/room/updatesettings.cpp"
    "sources/packets/in/userprofile/setavatar.cpp"
    "sources/packets/in/userprofile/setsignature.cpp"
    "sources/packets/in/userprofile/settitle.cpp"
    "sources/packets/in/userprofile/updatecampaign.cpp"
    "sources/packets/in/achievement.cpp"
    "sources/packets/in/chat.cpp"
    "sources/packets/in/favorite.cpp"
    "sources/packets/in/host.cpp"
    "sources/packets/in/option.cpp"
    "sources/packets/in/requestroomlist.cpp"
    "sources/packets/in/room.cpp"
    "sources/packets/in/userprofile.cpp"
    "sources/packets/out/achievement.cpp"
    "sources/packets/out/chat.cpp"
    "sources/packets/out/favorite.cpp"
    "sources/packets/out/host.cpp"
    "sources/packets/out/inventory.cpp"
    "sources/packets/out/lobby.cpp"
    "sources/packets/out/option.cpp"
    "sources/packets/out/room.cpp"
    "sources/packets/out/roomlist.cpp"
    "sources/packets/out/serverlist.cpp"
    "sources/packets/out/udp.cpp"
    "sources/packets/out/unlock.cpp"
    "sources/packets/out/userinfo.cpp"
    "sources/packets/out/userstart.cpp"
    "sources/packets/out/version.cpp"
    "sources/packets/achievementshared.cpp"
    "sources/packets/builder.cpp"
    "sources/packets/userinfoshared.cpp"
    "sources/packets/view.cpp"
    "sources/room/room.cpp"
    "sources/room/settings.cpp"
    "sources/room/slot.cpp"
    "sources/services/userservice.cpp"
    "sources/system/dialogbox.cpp"
    "sources/util/log.cpp"
    "sources/activesessions.cpp"
    "sources/cmdparser.cpp"
    "sources/clientsession.cpp"
    "sources/dependencies.cpp"
    "sources/holepunchserver.cpp"
    "sources/packetlogger.cpp"
    "sources/serverinstance.cpp")

set(LEET_HEADERS_BASE
    "headers/buffer/dynamicbuffer.hpp"
    "headers/buffer/staticbuffer.hpp"
    "headers/buffer/view.hpp"
    "headers/channel/channel.hpp"
    "headers/channel/channelgroup.hpp"
    "headers/channel/channelstorage.hpp"
    "headers/channel/shared.hpp"
    "headers/cso2/buymenu.hpp"
    "headers/cso2/cosmetics.hpp"
    "headers/cso2/definitions.hpp"
    "headers/cso2/inventory.hpp"
    "headers/cso2/loadout.hpp"
    "headers/cso2/shared.hpp"
    "headers/cso2/takedamageinfo.hpp"
    "headers/cso2/user.hpp"
    "headers/cso2/vector.hpp"
    "headers/generic/pingable.hpp"
    "headers/handlers.hpp"
    "headers/holepunch/inpacket/forward.hpp"
    "headers/holepunch/inpacket/punch.hpp"
    "headers/holepunch/inpacket.hpp"
    "headers/holepunch/outpacket.hpp"
    "headers/holepunch/shared.hpp"
    "headers/packets/in/achievement/campaign.hpp"
    "headers/packets/in/favorite/setcosmetics.hpp"
    "headers/packets/in/favorite/setloadout.hpp"
    "headers/packets/in/host/changeteam.hpp"
    "headers/packets/in/host/itemused.hpp"
    "headers/packets/in/host/onplayerscored.hpp"
    "headers/packets/in/host/onplayerbuyitem.hpp"
    "headers/packets/in/host/onplayerkilled.hpp"
    "headers/packets/in/host/onplayerspawn.hpp"
    "headers/packets/in/host/onroundend.hpp"
    "headers/packets/in/host/setinvobj.hpp"
    "headers/packets/in/option/setbuymenu.hpp"
    "headers/packets/in/room/countdown.hpp"
    "headers/packets/in/room/joinrequest.hpp"
    "headers/packets/in/room/newrequest.hpp"
    "headers/packets/in/room/setteam.hpp"
    "headers/packets/in/room/updatesettings.hpp"
    "headers/packets/in/userprofile/setavatar.hpp"
    "headers/packets/in/userprofile/setsignature.hpp"
    "headers/packets/in/userprofile/settitle.hpp"
    "headers/packets/in/userprofile/updatecampaign.hpp"
    "headers/packets/in/achievement.hpp"
    "headers/packets/in/chat.hpp"
    "headers/packets/in/favorite.hpp"
    "headers/packets/in/host.hpp"
    "headers/packets/in/login.hpp"
    "headers/packets/in/option.hpp"
    "headers/packets/in/room.hpp"
    "headers/packets/in/requestroomlist.hpp"
    "headers/packets/in/userprofile.hpp"
    "headers/packets/in/version.hpp"
    "headers/packets/out/achievement.hpp"
    "headers/packets/out/chat.hpp"
    "headers/packets/out/favorite.hpp"
    "headers/packets/out/host.hpp"
    "headers/packets/out/inventory.hpp"
    "headers/packets/out/lobby.hpp"
    "headers/packets/out/option.hpp"
    "headers/packets/out/room.hpp"
    "headers/packets/out/roomlist.hpp"
    "headers/packets/out/serverlist.hpp"
    "headers/packets/out/udp.hpp"
    "headers/packets/out/unlock.hpp"
    "headers/packets/out/userinfo.pp"
    "headers/packets/out/userstart.hpp"
    "headers/packets/out/version.hpp"
    "headers/packets/builder.hpp"
    "headers/packets/favoriteshared.hpp"
    "headers/packets/id.hpp"
    "headers/packets/optionshared.hpp"
    "headers/packets/view.hpp"
    "headers/room/match.hpp"
    "headers/room/room.hpp"
    "headers/room/settings.hpp"
    "headers/room/shared.hpp"
    "headers/room/slot.hpp"
    "headers/services/userservice.hpp"
    "headers/system/dialogbox.hpp"
    "headers/system/l10n.hpp"
    "headers/util/binary.hpp"
    "headers/util/fs.hpp"
    "headers/util/json_conv.hpp"
    "headers/util/log.hpp"
    "headers/util/randomstring.hpp"
    "headers/activesessions.hpp"
    "headers/cmdparser.hpp"
    "headers/clientsession.hpp"
    "headers/holepunchserver.hpp"
    "headers/packetlogger.hpp"
    "headers/serverinstance.hpp"
    "headers/serveroptions.hpp")

file(GLOB LEET_ALL_SOURCES ${LEET_SOURCES_BASE} ${LEET_HEADERS_BASE})

source_group("Source Files" FILES ${LEET_SOURCES_BASE} ${LEET_SOURCE_MAIN})
source_group("Header Files" FILES ${LEET_HEADERS_BASE})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

#
# setup dependencies
#

# threads, used by asio
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(Boost 1.73.0 REQUIRED)

# fmt
add_subdirectory("${LEET_LIB_FMT_DIR}")

# gsl
add_subdirectory("${LEET_LIB_GSL_DIR}")

#
# Add a static library to be shared between the main executable and the test
# executable
#
add_library(leet-server_static STATIC ${LEET_ALL_SOURCES})

#
# build the main executable
#
add_executable(leet-server ${LEET_SOURCE_MAIN})

#
# compiler flags
#

# enable warnings
if(MSVC)
  target_compile_options(leet-server_static PRIVATE /W4)
else()
  target_compile_options(leet-server_static PRIVATE -Wall -Wextra -Wconversion
                                                    -pedantic)
endif()

# enable coroutines on gcc
if(CMAKE_COMPILER_IS_GNUCXX)
  target_compile_options(leet-server_static PRIVATE -fcoroutines)
  target_compile_options(leet-server PRIVATE -fcoroutines)
endif()

#
# aditional definitions
#
target_compile_definitions(
  leet-server_static PRIVATE BOOST_BEAST_USE_STD_STRING_VIEW
                             BOOST_JSON_STANDALONE)

#
# Setup include directories
#
target_include_directories(
  leet-server_static
  PRIVATE "headers" ${Boost_INCLUDE_DIRS} "${LEET_LIB_FMT_DIR}/include"
          "${LEET_LIB_JSON_DIR}/include")

target_include_directories(leet-server PRIVATE "headers" ${Boost_INCLUDE_DIRS}
                                               "${LEET_LIB_FMT_DIR}/include")

#
# link libraries
#
target_link_libraries(leet-server_static fmt::fmt gsl::gsl-lite-v1
                      Threads::Threads)
target_link_libraries(leet-server leet-server_static)

#
# Build tests if the user wants them
#
if(LEET_BUILD_TESTS)
  message(STATUS "leet-server: Building tests")
  add_subdirectory(tests)
endif()
