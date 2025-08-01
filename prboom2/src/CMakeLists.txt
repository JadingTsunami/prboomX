
function(ApplySDL2NetOptions TARGET)
    if(SDL2_NET_FOUND)
        target_include_directories(${TARGET} PRIVATE ${SDL2_NET_INCLUDE_DIRS})
        target_link_libraries(${TARGET} PRIVATE ${SDL2_NET_LIBRARIES})
    endif()
endfunction()

# PrBoom main executable

set(COMMON_SRC
    am_map.c
    am_map.h
    doomdata.h
    doomdef.c
    doomdef.h
    doomstat.c
    doomstat.h
    doomtype.h
    dstrings.c
    dstrings.h
    d_deh.c
    d_deh.h
    d_englsh.h
    d_event.h
    d_items.c
    d_items.h
    d_main.c
    d_main.h
    d_net.h
    d_player.h
    d_think.h
    d_ticcmd.h
    e6y.c
    e6y.h
    f_finale.c
    f_finale.h
    f_finale2.c
    f_wipe.c
    f_wipe.h
    g_game.c
    g_game.h
    g_overflow.c
    g_overflow.h
    hu_lib.c
    hu_lib.h
    hu_stuff.c
    hu_stuff.h
    hu_tracers.c
    hu_tracers.h
    info.c
    info.h
    i_capture.c
    i_capture.h
    i_glob.c
    i_glob.h
    i_joy.h
    i_main.h
    i_network.h
    i_pcsound.c
    i_pcsound.h
    i_sound.h
    i_system.h
    i_video.h
    lprintf.c
    lprintf.h
    md5.c
    md5.h
    m_argv.c
    m_argv.h
    m_bbox.c
    m_bbox.h
    m_cheat.c
    m_cheat.h
    m_fixed.h
    m_menu.c
    m_menu.h
    m_misc.c
    m_misc.h
    m_io.c
    m_io.h
    m_random.c
    m_random.h
    m_swap.h
    protocol.h
    p_ceilng.c
    p_checksum.c
    p_checksum.h
    p_doors.c
    p_enemy.c
    p_enemy.h
    p_floor.c
    p_genlin.c
    p_inter.c
    p_inter.h
    p_lights.c
    p_map.c
    p_map.h
    p_maputl.c
    p_maputl.h
    p_mobj.c
    p_mobj.h
    p_plats.c
    p_pspr.c
    p_pspr.h
    p_saveg.c
    p_saveg.h
    p_setup.c
    p_setup.h
    p_sight.c
    p_spec.c
    p_spec.h
    p_switch.c
    p_telept.c
    p_tick.c
    p_tick.h
    p_user.c
    p_user.h
    r_bsp.c
    r_bsp.h
    r_data.c
    r_data.h
    r_defs.h
    r_demo.c
    r_demo.h
    r_draw.c
    r_draw.h
    r_filter.c
    r_filter.h
    r_fps.c
    r_fps.h
    r_main.c
    r_main.h
    r_patch.c
    r_patch.h
    r_plane.c
    r_plane.h
    r_segs.c
    r_segs.h
    r_sky.c
    r_sky.h
    r_state.h
    r_things.c
    r_things.h
    scanner.cpp
    scanner.h
    sc_man.c
    sc_man.h
    sounds.c
    sounds.h
    statdump.c
    statdump.h
    st_lib.c
    st_lib.h
    st_stuff.c
    st_stuff.h
    s_advsound.c
    s_advsound.h
    s_sound.c
    s_sound.h
    tables.c
    tables.h
    umapinfo.cpp
    umapinfo.h
    version.c
    version.h
    v_video.c
    v_video.h
    wi_stuff.c
    wi_stuff.h
    w_wad.c
    w_wad.h
    z_bmalloc.c
    z_bmalloc.h
    z_zone.c
    z_zone.h
    c_cmd.h
    c_cmd.c
    c_cvar.h
    c_cvar.c
    zip/zip.h
    zip/zip.c
)

set(NET_CLIENT_SRC
    d_client.c
)

if(HAVE_MMAP OR HAVE_CREATE_FILE_MAPPING)
    set(WAD_SRC w_mmap.c)
else()
    set(WAD_SRC w_memcache.c)
endif()

set(MUS2MID_SRC
    memio.c
    memio.h
    mus2mid.c
    mus2mid.h
)

set(SDLDOOM_SOURCES
    SDL/i_joy.c
    SDL/i_main.c
    SDL/i_network.c
    SDL/i_sound.c
    SDL/i_sshot.c
    SDL/i_system.c
    SDL/i_video.c
)

set(PCSOUND_SOURCES
    PCSOUND/pcsound.c
    PCSOUND/pcsound.h
    PCSOUND/pcsound_linux.c
    PCSOUND/pcsound_sdl.c
    PCSOUND/pcsound_win32.c
)

set(TEXTSCREEN_SOURCES
    TEXTSCREEN/doomkeys.h
    TEXTSCREEN/txt_main.h
    TEXTSCREEN/txt_font.h
    TEXTSCREEN/txt_largefont.h
    TEXTSCREEN/txt_sdl.c
    TEXTSCREEN/txt_sdl.h
    TEXTSCREEN/txt_smallfont.h
)

set(DOOMMUSIC_SOURCES
    MUSIC/dbopl.c
    MUSIC/dbopl.h
    MUSIC/dumbplayer.c
    MUSIC/dumbplayer.h
    MUSIC/flplayer.c
    MUSIC/flplayer.h
    MUSIC/madplayer.c
    MUSIC/madplayer.h
    MUSIC/midifile.c
    MUSIC/midifile.h
    MUSIC/musicplayer.h
    MUSIC/opl.c
    MUSIC/opl.h
    MUSIC/oplplayer.c
    MUSIC/oplplayer.h
    MUSIC/opl_queue.c
    MUSIC/opl_queue.h
    MUSIC/portmidiplayer.c
    MUSIC/portmidiplayer.h
    MUSIC/alsaplayer.c
    MUSIC/alsaplayer.h
    MUSIC/vorbisplayer.c
    MUSIC/vorbisplayer.h
)

set(EXTRA_FILES
    r_drawcolpipeline.inl
    r_drawcolumn.inl
    r_drawflush.inl
    r_drawspan.inl
)

set(PRBOOM_PLUS_SOURCES
    ${COMMON_SRC}
    ${NET_CLIENT_SRC}
    ${WAD_SRC}
    ${MUS2MID_SRC}
    ${SDLDOOM_SOURCES}
    ${PCSOUND_SOURCES}
    ${TEXTSCREEN_SOURCES}
    ${DOOMMUSIC_SOURCES}
    ${EXTRA_FILES}
)

function(AddGameExecutable TARGET SOURCES)
    if(OPENGL_FOUND AND OPENGL_GLU_FOUND)
        set(SOURCES
            ${SOURCES}
            gl_clipper.c
            gl_detail.c
            gl_drawinfo.c
            gl_fbo.c
            gl_gamma.c
            gl_hires.c
            gl_hqresize.c
            gl_intern.h
            gl_light.c
            gl_main.c
            gl_map.c
            gl_missingtexture.c
            gl_opengl.c
            gl_opengl.h
            gl_preprocess.c
            gl_shader.c
            gl_shadow.c
            gl_sky.c
            gl_struct.h
            gl_texture.c
            gl_vertex.c
            gl_wipe.c
        )
    endif()

    if(WIN32)
        add_definitions("-DUSE_WIN32_PCSOUND_DRIVER -DUSE_WINDOWS_LAUNCHER")
        set(SOURCES
            ${SOURCES}
            ../ICONS/icons.rc
            e6y_launcher.c
            e6y_launcher.h
            SDL/SDL_windows_main.c
        )
        if(MSVC)
            set(SOURCES
                ${SOURCES}
                WIN/win_opendir.c
                WIN/win_opendir.h
            )
        endif()
    endif()

    add_definitions("-DUSE_EXPERIMENTAL_MUSIC")

    add_executable(${TARGET} WIN32 ${SOURCES})
    target_include_directories(${TARGET} PRIVATE
        ${SDL2_INCLUDE_DIRS}
        ${CMAKE_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
    )
    target_link_libraries(${TARGET} PRIVATE
        ${SDL2_LIBRARIES}
    )
    if(WIN32)
        target_link_libraries(${TARGET} PRIVATE
            winmm
            comctl32
        )
    endif()
    set_target_properties(${TARGET} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PRBOOM_OUTPUT_PATH}
    )
    if(SDL2_IMAGE_FOUND)
        target_include_directories(${TARGET} PRIVATE ${SDL2_IMAGE_INCLUDE_DIRS})
        target_link_libraries(${TARGET} PRIVATE ${SDL2_IMAGE_LIBRARIES})
    endif()
    if(SDL2_MIXER_FOUND)
        target_include_directories(${TARGET} PRIVATE ${SDL2_MIXER_INCLUDE_DIRS})
        target_link_libraries(${TARGET} PRIVATE ${SDL2_MIXER_LIBRARIES})
    endif()
    if(PCREPOSIX_FOUND)
        target_include_directories(${TARGET} PRIVATE ${PCRE_INCLUDE_DIR})
        target_link_libraries(${TARGET} PRIVATE ${PCREPOSIX_LIBRARIES})
    endif()
    ApplySDL2NetOptions(${TARGET})
    if(ZLIB_FOUND)
        target_include_directories(${TARGET} PRIVATE ${ZLIB_INCLUDE_DIRS})
        target_link_libraries(${TARGET} PRIVATE ${ZLIB_LIBRARIES})
    endif()
    if(LIBMAD_FOUND)
        target_include_directories(${TARGET} PRIVATE ${LIBMAD_INCLUDE_DIRS})
        target_link_libraries(${TARGET} PRIVATE ${LIBMAD_LIBRARIES})
    endif()
    if(FLUIDSYNTH_FOUND)
        target_include_directories(${TARGET} PRIVATE ${FLUIDSYNTH_INCLUDE_DIR})
        target_link_libraries(${TARGET} PRIVATE ${FLUIDSYNTH_LIBRARIES})
    endif()
    if(DUMB_FOUND)
        target_include_directories(${TARGET} PRIVATE ${DUMB_INCLUDE_DIR})
        target_link_libraries(${TARGET} PRIVATE ${DUMB_LIBRARY})
    endif()
    if(VORBIS_FOUND)
        target_include_directories(${TARGET} PRIVATE ${VORBIS_INCLUDE_DIR})
        target_link_libraries(${TARGET} PRIVATE ${VORBISFILE_LIBRARY})
    endif()
    if(PortMidi_FOUND)
        target_include_directories(${TARGET} PRIVATE ${PORTMIDI_INCLUDE_DIR})
        target_link_libraries(${TARGET} PRIVATE ${PORTMIDI_LIBRARIES})
    endif()
    if(ALSA_FOUND)
        target_include_directories(${TARGET} PRIVATE ${ALSA_INCLUDE_DIR})
        target_link_libraries(${TARGET} PRIVATE ${ASOUND_LIBRARY})
    endif()
    add_dependencies(${TARGET} prboomwad)

    if(MSVC)
        set_target_properties(${TARGET} PROPERTIES
            LINK_FLAGS "/MANIFEST:NO"
        )
        add_custom_command(TARGET ${TARGET} POST_BUILD
            COMMAND "mt.exe" -manifest \"${CMAKE_CURRENT_SOURCE_DIR}\\..\\ICONS\\prboomX.exe.manifest\" -outputresource:\"$<TARGET_FILE:prboomX>\"\;\#1
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${WAD_DATA_PATH} $<TARGET_FILE_DIR:prboomX>
        )
    endif()

    install(TARGETS ${TARGET} COMPONENT "Game executable" RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")

    if(OPENGL_FOUND AND OPENGL_GLU_FOUND)
        target_compile_definitions(${TARGET} PRIVATE
            GL_DOOM
            USE_SHADERS
            USE_FBO_TECHNIQUE
            USE_GLU_IMAGESCALE
            USE_GLU_MIPMAP
            USE_GLU_TESS
        )
        target_link_libraries(${TARGET} PRIVATE
            ${OPENGL_gl_LIBRARY}
            ${OPENGL_glu_LIBRARY}
        )
    endif()
endfunction()

AddGameExecutable(prboomX "${PRBOOM_PLUS_SOURCES}")


# PrBoomX server executable

option(BUILD_SERVER "Build PrBoomX server executable" ON)

if(BUILD_SERVER AND SDL2_NET_FOUND)
    set(PRBOOM_PLUS_GAME_SERVER_SOURCES
        d_server.c
        protocol.h
        SDL/i_network.c
    )
    if(MSVC)
        set(PRBOOM_PLUS_GAME_SERVER_SOURCES
            ${PRBOOM_PLUS_GAME_SERVER_SOURCES}
            SDL/i_system.c
        )
    else()
        set(PRBOOM_PLUS_GAME_SERVER_SOURCES
            ${PRBOOM_PLUS_GAME_SERVER_SOURCES}
            POSIX/i_system.c
        )
    endif()

    add_executable(prboomX-game-server ${PRBOOM_PLUS_GAME_SERVER_SOURCES})
    target_include_directories(prboomX-game-server PRIVATE
        ${SDL2_INCLUDE_DIRS}
        ${CMAKE_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
    )
    target_link_libraries(prboomX-game-server PRIVATE
        ${SDL2_LIBRARIES}
    )
    set_target_properties(prboomX-game-server PROPERTIES
        COMPILE_DEFINITIONS PRBOOM_SERVER
        RUNTIME_OUTPUT_DIRECTORY ${PRBOOM_OUTPUT_PATH}
    )
    ApplySDL2NetOptions(prboomX-game-server)
    install(TARGETS prboomX-game-server COMPONENT "Game server executable" RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
endif()


# PrBoomX macOS launcher
# Deprecated
#if(APPLE)
#    set(LAUNCHER_SOURCES
#        MAC/ANSIString.m
#        MAC/ConsoleController.h
#        MAC/ConsoleController.m
#        MAC/DrawerButton.h
#        MAC/DrawerButton.m
#        MAC/FileButtonController.h
#        MAC/FileButtonController.m
#        MAC/LauncherApp.h
#        MAC/LauncherApp.m
#        MAC/LauncherMain.m
#        MAC/ResolutionDataSource.h
#        MAC/ResolutionDataSource.m
#        MAC/UKFileWatcher.h
#        MAC/UKKQueue.h
#        MAC/UKKQueue.m
#        MAC/UKMainThreadProxy.h
#        MAC/UKMainThreadProxy.m
#        MAC/WadViewController.h
#        MAC/WadViewController.m
#    )
#
#    set(LAUNCHER_RESOURCES
#        MAC/Launcher.icns
#        MAC/PrBoom.icns
#        MAC/PrBoom.sdef
#    )
#    set_source_files_properties(${LAUNCHER_RESOURCES} PROPERTIES
#        MACOSX_PACKAGE_LOCATION Resources
#    )
#
#    set(LAUNCHER_MAINMENU_RESOURCES
#        MAC/English.lproj/MainMenu.nib/classes.nib
#        MAC/English.lproj/MainMenu.nib/info.nib
#        MAC/English.lproj/MainMenu.nib/keyedobjects.nib
#    )
#    set_source_files_properties(${LAUNCHER_MAINMENU_RESOURCES} PROPERTIES
#        MACOSX_PACKAGE_LOCATION Resources/English.lproj/MainMenu.nib
#    )
#
#    add_executable(prboomX-launcher MACOSX_BUNDLE
#        ${LAUNCHER_SOURCES}
#        ${LAUNCHER_RESOURCES}
#        ${LAUNCHER_MAINMENU_RESOURCES}
#    )
#    target_include_directories(prboomX-launcher PRIVATE
#        ${CMAKE_BINARY_DIR}
#        ${CMAKE_CURRENT_SOURCE_DIR}
#    )
#    set_target_properties(prboomX-launcher PROPERTIES
#        LINK_FLAGS "-framework Cocoa"
#        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/MAC/Info.plist"
#        OUTPUT_NAME "Launcher"
#        RUNTIME_OUTPUT_DIRECTORY ${PRBOOM_OUTPUT_PATH}
#        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ""
#    )
#    add_dependencies(prboomX-launcher prboomX)
#
#    set(BUNDLE_GAME_EXECUTABLE prboomX)
#
#    add_custom_command(TARGET prboomX-launcher POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${BUNDLE_GAME_EXECUTABLE}> $<TARGET_FILE_DIR:prboomX-launcher>/PrBoomX
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${WAD_DATA_PATH} $<TARGET_FILE_DIR:prboomX-launcher>
#    )
#
#    if(BUILD_SERVER AND SDL2_NET_FOUND)
#        add_dependencies(prboomX-launcher prboomX-game-server)
#        add_custom_command(TARGET prboomX-launcher POST_BUILD
#            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:prboomX-game-server> $<TARGET_FILE_DIR:prboomX-launcher>
#        )
#    endif()
#endif()
