cmake_minimum_required(VERSION 3.5)
project(CombatC VERSION 1.1.0) 

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules/")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/release)

FIND_PACKAGE(Allegro REQUIRED)

IF(ALLEGRO_FOUND)
   INCLUDE_DIRECTORIES(${ALLEGRO_INCLUDE_DIR})
ENDIF(ALLEGRO_FOUND)

add_executable(combat core/src/combat.c modules/src/collision.c modules/src/drawer.c modules/src/obstacle.c modules/src/tank.c screens/src/game.c screens/src/score.c)

find_library(MATH_LIBRARY m)
if(MATH_LIBRARY)
    target_link_libraries(combat PUBLIC ${MATH_LIBRARY} ${ALLEGRO_LIBRARY})
endif()