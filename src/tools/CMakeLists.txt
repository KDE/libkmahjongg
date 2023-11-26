# SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause

add_executable(renderelement renderelement.cpp)
target_link_libraries(renderelement Qt::Svg)
