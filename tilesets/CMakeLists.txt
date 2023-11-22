# SPDX-FileCopyrightText: 2021, 2023 Friedrich W. H. Kossebau <kossebau@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause

function(list_with_tileset_rendering_check id old_file new_file)
    # symbolic target for cheking this file, always outdated
    set(check_rendering_output "check_rendering_${old_file}")
    set_property(SOURCE ${check_rendering_output} PROPERTY SYMBOLIC 1)

    add_custom_command(
        OUTPUT ${check_rendering_output}
        COMMAND ${CMAKE_COMMAND}
        ARGS
            "-DID=${id}"
            "-DWORKING_DIR=${CMAKE_CURRENT_BINARY_DIR}"
            "-DRENDERELEMENT=$<TARGET_FILE:renderelement>"
            "-DCOMPARE=${ImageMagick_compare_EXECUTABLE}"
            "-DOLD_FILE=${CMAKE_CURRENT_SOURCE_DIR}/${old_file}"
            "-DNEW_FILE=${new_file}"
            -P ${CMAKE_CURRENT_SOURCE_DIR}/CompareTilesetsSVGs.cmake
        COMMENT "Checking SVG of ${id}"
    )

    # add to general target
    set(target_check_rendering_output "check_rendering_tileset_${id}")
    add_custom_target(${target_check_rendering_output} DEPENDS ${check_rendering_output})
    add_dependencies(check_rendering ${target_check_rendering_output})
endfunction()

function(install_tileset id)
    cmake_parse_arguments(ARGS "NO_CLEANING" "" "" ${ARGN})

    set(generate_args)
    if (ARGS_NO_CLEANING)
        list(APPEND generate_args NO_CLEANING)
    endif()

    set(svgz "${CMAKE_CURRENT_BINARY_DIR}/${id}.svgz")
    generate_svgz(${id}.svg ${svgz} "tileset-" ${generate_args})
    if(BUILD_SVG_CHECKS)
        list_with_tileset_rendering_check(${id} ${id}.svg ${svgz})
    endif()
    install(
        FILES
            ${id}.desktop
            ${svgz}
        DESTINATION ${KDE_INSTALL_DATADIR}/kmahjongglib/tilesets
    )
endfunction()

install_tileset(default)
install_tileset(classic)
#install_tileset(bamboo)
install_tileset(traditional)
install_tileset(alphabet)
install_tileset(egypt)
install_tileset(jade)
