function(install_svg_background id)
    install(
        FILES
            ${id}.desktop
            ${id}.svg
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
