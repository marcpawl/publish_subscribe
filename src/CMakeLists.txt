option(CPP_STARTER_USE_QT "Enable compilation of QT sample" OFF)
option(CPP_STARTER_USE_FLTK "Enable compilation of FLTK sample" OFF)
option(CPP_STARTER_USE_GTKMM "Enable compilation of GTKMM sample" OFF)
option(CPP_STARTER_USE_IMGUI "Enable compilation of ImGui sample" OFF)
option(CPP_STARTER_USE_NANA "Enable compilation of Nana GUI sample" OFF)
option(CPP_STARTER_USE_SDL "Enable compilation of SDL sample" OFF)


# sdl
if(CPP_STARTER_USE_SDL)
  message("Using SDL2")
  add_subdirectory(sdl)
endif()

# qt
if(CPP_STARTER_USE_QT)
  message("Using Qt")
  add_subdirectory(qt)
endif()

# fltk test
if(CPP_STARTER_USE_FLTK)
  message("Using FLTK")
  add_subdirectory(fltk)
endif()

# gtkmm test
if(CPP_STARTER_USE_GTKMM)
  message("Using gtkmm")
  add_subdirectory(gtkmm)
endif()

# imgui example
if(CPP_STARTER_USE_IMGUI)
  message("Using Dear ImGui")
  add_subdirectory(imgui)
endif()

# Nana
if(CPP_STARTER_USE_NANA)
  message("Using Nana")
  add_subdirectory(nana)
endif()

set_property(GLOBAL PROPERTY CXX_STANDARD 17)
add_compile_options("-O3")

# Generic test that uses conan libs
add_library(lpubsub  legacy.cpp subscription.cpp weak.cpp)
add_executable(pubsub main.cpp )
target_link_libraries(
  pubsub
  PUBLIC
  ${LIBS}
  lpubsub
  PRIVATE project_options
          project_warnings
          CONAN_PKG::benchmark
  )
target_link_libraries(
  lpubsub
  PUBLIC
  ${LIBS}
  PRIVATE project_options
          project_warnings
  )
