function(install_tileset id)
    install(
        FILES
            ${id}.desktop
            ${id}.svg
            ${id}.copyright
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
