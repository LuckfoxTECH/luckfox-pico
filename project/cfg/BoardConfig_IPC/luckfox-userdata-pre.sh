#!/bin/bash

function lf_rm() {
    for file in "$@"; do
        if [ -e "$file" ]; then
            echo "Deleting: $file"
            rm -rf "$file"  
        #else
            #echo "File not found: $file" 
        fi
    done
}

# remove unused files
function remove_data()
{
    lf_rm $RK_PROJECT_PACKAGE_USERDATA_DIR/*.sh
    lf_rm $RK_PROJECT_PACKAGE_USERDATA_DIR/*.bmp
}

#=========================
# run
#=========================
remove_data