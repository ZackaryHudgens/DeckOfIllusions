file(GLOB DECK_SOURCES *.cpp)

add_executable(deck-of-illusions ${DECK_SOURCES})

message(${URSINE_INCLUDE_DIRS})

target_link_libraries(deck-of-illusions PUBLIC
                      ${URSINE_LIBRARIES})
target_include_directories(deck-of-illusions PUBLIC
                           ${URSINE_INCLUDE_DIRS})

install(TARGETS deck-of-illusions DESTINATION ${INSTALL_DIR})
install(DIRECTORY resources DESTINATION ${INSTALL_DIR})
