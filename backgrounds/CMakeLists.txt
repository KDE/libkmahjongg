# SPDX-FileCopyrightText: 2021 Friedrich W. H. Kossebau <kossebau@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause

function(install_svg_background id)
    set(svgz "${CMAKE_CURRENT_BINARY_DIR}/${id}.svgz")
    generate_svgz(${id}.svg ${svgz} "background-")
    install(
        FILES
            ${id}.desktop
            ${svgz}
            ${id}.copyright
        DESTINATION  ${KDE_INSTALL_DATADIR}/kmahjongglib/backgrounds
    )
endfunction()

install_svg_background(default)

install_svg_background(chinese_landscape)

install_svg_background(egyptian)

install_svg_background(summerfield)

install_svg_background(wood_light)

##plain color
install(FILES color_plain.desktop  DESTINATION  ${KDE_INSTALL_DATADIR}/kmahjongglib/backgrounds)
