# SPDX-FileCopyrightText: 2021, 2023 Friedrich W. H. Kossebau <kossebau@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause

function(list_with_background_rendering_check id old_file new_file)
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
            -P ${CMAKE_CURRENT_SOURCE_DIR}/CompareBackgroundSVGs.cmake
        COMMENT "Checking SVG of ${id}"
    )

    # add to general target
    set(target_check_rendering_output "check_rendering_background_${id}")
    add_custom_target(${target_check_rendering_output} DEPENDS ${check_rendering_output})
    add_dependencies(check_rendering ${target_check_rendering_output})
endfunction()

function(install_svg_background id)
    cmake_parse_arguments(ARGS "NO_CLEANING" "" "" ${ARGN})

    set(generate_args)
    if (ARGS_NO_CLEANING)
        list(APPEND generate_args NO_CLEANING)
    endif()

    set(svgz "${CMAKE_CURRENT_BINARY_DIR}/${id}.svgz")
    generate_svgz(${id}.svg ${svgz} "background-" ${generate_args})
    if(BUILD_SVG_CHECKS)
        list_with_background_rendering_check(${id} ${id}.svg ${svgz})
    endif()
    install(
        FILES
            ${id}.desktop
            ${svgz}
            ${id}.copyright
        DESTINATION  ${KDE_INSTALL_DATADIR}/kmahjongglib/backgrounds
    )
endfunction()

# svgcleaner seems to remove something needed, not yet investigated
install_svg_background(default NO_CLEANING)

install_svg_background(chinese_landscape)

install_svg_background(egyptian)

install_svg_background(summerfield)

install_svg_background(wood_light)

##plain color
install(FILES color_plain.desktop  DESTINATION  ${KDE_INSTALL_DATADIR}/kmahjongglib/backgrounds)
