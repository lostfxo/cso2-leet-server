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
set(LEET_LIB_JSON_DIR "${LEET_LIBS_DIR}/json")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#
# add source files to the project
#
set(LEET_SOURCE_MAIN "src/main.cpp")

set(LEET_SOURCES_BASE
    "src/buffer/dynamicbuffer.cpp"
    "src/buffer/staticbuffer.cpp"
    "src/buffer/view.cpp"
    "src/channel/channel.cpp"
    "src/channel/channelgroup.cpp"
    "src/channel/channelstorage.cpp"
    "src/channel/shared.cpp"
    "src/cso2/buymenu.cpp"
    "src/cso2/cosmetics.cpp"
    "src/cso2/definitions.cpp"
    "src/cso2/inventory.cpp"
    "src/cso2/loadout.cpp"
    "src/cso2/user.cpp"
    "src/handlers/achievementhandler.cpp"
    "src/handlers/chathandler.cpp"
    "src/handlers/channelhandler.cpp"
    "src/handlers/favoritehandler.cpp"
    "src/handlers/hosthandler.cpp"
    "src/handlers/loginhandler.cpp"
    "src/handlers/optionhandler.cpp"
    "src/handlers/roomhandler.cpp"
    "src/handlers/userprofilehandler.cpp"
    "src/handlers/versionhandler.cpp"
    "src/holepunch/inpacket/forward.cpp"
    "src/holepunch/inpacket/punch.cpp"
    "src/holepunch/inpacket.cpp"
    "src/holepunch/outpacket.cpp"
    "src/packets/in/achievement/campaign.cpp"
    "src/packets/in/favorite/setcosmetics.cpp"
    "src/packets/in/favorite/setloadout.cpp"
    "src/packets/in/host/changeteam.cpp"
    "src/packets/in/host/itemused.cpp"
    "src/packets/in/host/onplayerscored.cpp"
    "src/packets/in/host/onplayerbuyitem.cpp"
    "src/packets/in/host/onplayerkilled.cpp"
    "src/packets/in/host/onplayerspawn.cpp"
    "src/packets/in/host/onroundend.cpp"
    "src/packets/in/host/setinvobj.cpp"
    "src/packets/in/option/setbuymenu.cpp"
    "src/packets/in/room/countdown.cpp"
    "src/packets/in/room/joinrequest.cpp"
    "src/packets/in/room/newroom.cpp"
    "src/packets/in/room/setteam.cpp"
    "src/packets/in/room/updatesettings.cpp"
    "src/packets/in/userprofile/setavatar.cpp"
    "src/packets/in/userprofile/setsignature.cpp"
    "src/packets/in/userprofile/settitle.cpp"
    "src/packets/in/userprofile/updatecampaign.cpp"
    "src/packets/in/achievement.cpp"
    "src/packets/in/chat.cpp"
    "src/packets/in/favorite.cpp"
    "src/packets/in/host.cpp"
    "src/packets/in/option.cpp"
    "src/packets/in/requestroomlist.cpp"
    "src/packets/in/room.cpp"
    "src/packets/in/userprofile.cpp"
    "src/packets/out/achievement.cpp"
    "src/packets/out/chat.cpp"
    "src/packets/out/custom.cpp"
    "src/packets/out/favorite.cpp"
    "src/packets/out/host.cpp"
    "src/packets/out/inventory.cpp"
    "src/packets/out/lobby.cpp"
    "src/packets/out/option.cpp"
    "src/packets/out/room.cpp"
    "src/packets/out/roomlist.cpp"
    "src/packets/out/serverlist.cpp"
    "src/packets/out/udp.cpp"
    "src/packets/out/unlock.cpp"
    "src/packets/out/userinfo.cpp"
    "src/packets/out/userstart.cpp"
    "src/packets/out/version.cpp"
    "src/packets/achievementshared.cpp"
    "src/packets/builder.cpp"
    "src/packets/userinfoshared.cpp"
    "src/packets/view.cpp"
    "src/room/room.cpp"
    "src/room/settings.cpp"
    "src/room/slot.cpp"
    "src/services/userservice.cpp"
    "src/system/dialogbox.cpp"
    "src/util/log.cpp"
    "src/util/randomstring.cpp"
    "src/activesessions.cpp"
    "src/cmdparser.cpp"
    "src/clientsession.cpp"
    "src/dependencies.cpp"
    "src/holepunchserver.cpp"
    "src/packetlogger.cpp"
    "src/serverinstance.cpp")

if(WIN32)
  set(LEET_SOURCES_BASE "src/platform/win32.cpp" ${LEET_SOURCES_BASE})
elseif(UNIX)
  set(LEET_SOURCES_BASE "src/platform/unix.cpp" ${LEET_SOURCES_BASE})
endif()

set(LEET_HEADERS_BASE
    "src/buffer/dynamicbuffer.hpp"
    "src/buffer/staticbuffer.hpp"
    "src/buffer/view.hpp"
    "src/channel/channel.hpp"
    "src/channel/channelgroup.hpp"
    "src/channel/channelstorage.hpp"
    "src/channel/shared.hpp"
    "src/cso2/buymenu.hpp"
    "src/cso2/cosmetics.hpp"
    "src/cso2/definitions.hpp"
    "src/cso2/inventory.hpp"
    "src/cso2/loadout.hpp"
    "src/cso2/shared.hpp"
    "src/cso2/takedamageinfo.hpp"
    "src/cso2/user.hpp"
    "src/cso2/vector.hpp"
    "src/generic/pingable.hpp"
    "src/handlers.hpp"
    "src/holepunch/inpacket/forward.hpp"
    "src/holepunch/inpacket/punch.hpp"
    "src/holepunch/inpacket.hpp"
    "src/holepunch/outpacket.hpp"
    "src/holepunch/shared.hpp"
    "src/packets/in/achievement/campaign.hpp"
    "src/packets/in/favorite/setcosmetics.hpp"
    "src/packets/in/favorite/setloadout.hpp"
    "src/packets/in/host/changeteam.hpp"
    "src/packets/in/host/itemused.hpp"
    "src/packets/in/host/onplayerscored.hpp"
    "src/packets/in/host/onplayerbuyitem.hpp"
    "src/packets/in/host/onplayerkilled.hpp"
    "src/packets/in/host/onplayerspawn.hpp"
    "src/packets/in/host/onroundend.hpp"
    "src/packets/in/host/setinvobj.hpp"
    "src/packets/in/option/setbuymenu.hpp"
    "src/packets/in/room/countdown.hpp"
    "src/packets/in/room/joinrequest.hpp"
    "src/packets/in/room/newrequest.hpp"
    "src/packets/in/room/setteam.hpp"
    "src/packets/in/room/updatesettings.hpp"
    "src/packets/in/userprofile/setavatar.hpp"
    "src/packets/in/userprofile/setsignature.hpp"
    "src/packets/in/userprofile/settitle.hpp"
    "src/packets/in/userprofile/updatecampaign.hpp"
    "src/packets/in/achievement.hpp"
    "src/packets/in/chat.hpp"
    "src/packets/in/favorite.hpp"
    "src/packets/in/host.hpp"
    "src/packets/in/login.hpp"
    "src/packets/in/option.hpp"
    "src/packets/in/room.hpp"
    "src/packets/in/requestroomlist.hpp"
    "src/packets/in/userprofile.hpp"
    "src/packets/in/version.hpp"
    "src/packets/out/achievement.hpp"
    "src/packets/out/chat.hpp"
    "src/packets/out/custom.hpp"
    "src/packets/out/favorite.hpp"
    "src/packets/out/host.hpp"
    "src/packets/out/inventory.hpp"
    "src/packets/out/lobby.hpp"
    "src/packets/out/option.hpp"
    "src/packets/out/room.hpp"
    "src/packets/out/roomlist.hpp"
    "src/packets/out/serverlist.hpp"
    "src/packets/out/udp.hpp"
    "src/packets/out/unlock.hpp"
    "src/packets/out/userinfo.pp"
    "src/packets/out/userstart.hpp"
    "src/packets/out/version.hpp"
    "src/packets/builder.hpp"
    "src/packets/favoriteshared.hpp"
    "src/packets/id.hpp"
    "src/packets/optionshared.hpp"
    "src/packets/view.hpp"
    "src/room/match.hpp"
    "src/room/room.hpp"
    "src/room/settings.hpp"
    "src/room/shared.hpp"
    "src/room/slot.hpp"
    "src/services/userservice.hpp"
    "src/system/dialogbox.hpp"
    "src/system/l10n.hpp"
    "src/util/binary.hpp"
    "src/util/fs.hpp"
    "src/util/json_conv.hpp"
    "src/util/json_fwd.hpp"
    "src/util/log.hpp"
    "src/util/number.hpp"
    "src/util/randomstring.hpp"
    "src/activesessions.hpp"
    "src/cmdparser.hpp"
    "src/clientsession.hpp"
    "src/holepunchserver.hpp"
    "src/packetlogger.hpp"
    "src/platform.hpp"
    "src/serverinstance.hpp"
    "src/serveroptions.hpp")

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
  PRIVATE "src" ${Boost_INCLUDE_DIRS} "${LEET_LIB_FMT_DIR}/include"
          "${LEET_LIB_JSON_DIR}/include")

target_include_directories(leet-server PRIVATE "src" ${Boost_INCLUDE_DIRS}
                                               "${LEET_LIB_FMT_DIR}/include")

#
# link libraries
#
target_link_libraries(leet-server_static fmt::fmt Threads::Threads)
target_link_libraries(leet-server leet-server_static)

#
# Build tests if the user wants them
#
if(LEET_BUILD_TESTS)
  message(STATUS "leet-server: Building tests")
  add_subdirectory(tests)
endif()
