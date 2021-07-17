function(install_tileset id)
    set(svgz "${CMAKE_CURRENT_BINARY_DIR}/${id}.svgz")
    generate_svgz(${id}.svg ${svgz} "tileset-")
    install(
        FILES
            ${id}.desktop
            ${svgz}
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
