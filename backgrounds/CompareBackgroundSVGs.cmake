# SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause

set(id ${ID})
set(render_tool ${RENDERELEMENT})
set(rcompare_tool ${COMPARE})
set(old_file ${OLD_FILE})
set(new_file ${NEW_FILE})
set(work_dir "${WORKING_DIR}/${id}/check")

if (NOT EXISTS "${old_file}")
    message(FATAL_ERROR "Old file ${old_file} does not exist")
endif()
if (NOT EXISTS "${new_file}")
    message(FATAL_ERROR "New file ${new_file} does not exist")
endif()

# ensure working dir
make_directory(${work_dir})

# generate PNG for original
set(old_png_file "old.png")
execute_process(
    COMMAND ${render_tool} ${old_file} "" 200 200 ${old_png_file}
    WORKING_DIRECTORY ${work_dir}
)
# generate PNG for modified
set(new_png_file "new.png")
execute_process(
    COMMAND ${render_tool} ${new_file} "" 200 200 ${new_png_file}
    WORKING_DIRECTORY ${work_dir}
)

# compare
execute_process(
    COMMAND ${rcompare_tool}
        -metric MAE
        ${old_png_file}
        ${new_png_file}
        /dev/null # no use for difference image
    WORKING_DIRECTORY ${work_dir}
    OUTPUT_VARIABLE compare_output
    ERROR_VARIABLE compare_output
)
string(REGEX MATCH "^[0-9]*(\\.[0-9]*)?" compare_result "${compare_output}")
if("${compare_result}" STRGREATER 0)
    message(STATUS "Difference for background ${id}: ${compare_result}")
endif()
