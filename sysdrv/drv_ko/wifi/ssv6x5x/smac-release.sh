#!/bin/bash
release_options=("From wifi [trunk] -> [regular-tag]" \
              "From wifi [trunk] -> [smac-release-tag]" \
              "From wifi [regular-tag] -> [smac-release-tag] -> [QA_tags]" \
              "From wifi [smac-release-tag] -> [QA_tags]" \
              "From wifi [QA_tags] -> [software-mac]")


echo ""
echo "smac-release [Target Version tag] [JIRA ID] [Regular Version]"
echo ""

rm -rf $1
rm -rf $3 

select opt in "${release_options[@]}" "Quit"; do 
    case "$REPLY" in

    1 ) echo "${release_options[$REPLY-1]} is option";
        if [ "$1" != "" ]; then
        if [ "$3" != "" ]; then
        rm -rf $1
        rm -rf $3
        svn copy http://192.168.15.30/svn/software/wifi/trunk/CABRIO-E/host_drivers/Linux \
            http://192.168.15.30/svn/software/wifi/tag/regular-tag/$3 -m "release $3"
        svn co http://192.168.15.30/svn/software/wifi/tag/regular-tag/$3
        cd $3/ssv6x5x
        ./gen-version.sh
        svn commit * -m "$2 release $3"
        else
        echo "No WSD-XX or NO WSP-XX parameter"
        fi
        echo "Done!"
        else
        echo "No verion parameter!"
        fi
        break;;
    2 ) echo "${release_options[$REPLY-1]} is option";
        if [ "$1" != "" ]; then
        if [ "$2" != "" ]; then
        rm -rf $1
        svn copy http://192.168.15.30/svn/software/wifi/trunk/CABRIO-E/host_drivers/Linux \
            http://192.168.15.30/svn/software/wifi/tag/smac-release-tag/$1 -m "release $1"
        svn co http://192.168.15.30/svn/software/wifi/tag/smac-release-tag/$1
        cd $1/ssv6x5x
        ./gen-version.sh
        svn commit * -m "$2 release $1"
        else
        echo "No WSD-XX or NO WSP-XX parameter"
        fi
        echo "Done!"
        else
        echo "No verion parameter!"
        fi
        break;;
    3 ) echo "${release_options[$REPLY-1]} is option";
        if [ "$1" != "" ]; then
        if [ "$3" != "" ]; then
        rm -rf $1
        rm -rf $3
        svn copy http://192.168.15.30/svn/software/wifi/tag/regular-tag/$3 \
            http://192.168.15.30/svn/software/wifi/tag/smac-release-tag/$1 -m "release $1"
        svn co http://192.168.15.30/svn/software/wifi/tag/smac-release-tag/$1
        cd $1/ssv6x5x
        ./gen-version.sh
        script/release
        cd ../..
        svn import $1 http://192.168.15.30/svn/software/QA_tags/Ubuntu-PC/$1 -m "$2 release $1"
        rm -rf $1
        svn co http://192.168.15.30/svn/software/QA_tags/Ubuntu-PC/$1
        cd $1/ssv6x5x
        ./gen-version.sh
        svn commit * -m "$2 release $1"
        else
        echo "No WSD-XX or NO WSP-XX parameter"
        fi
        echo "Done!"
        else
        echo "No verion parameter!"
        fi
        break;;
    4 ) echo "${release_options[$REPLY-1]} is option";
        if [ "$1" != "" ]; then
        if [ "$2" != "" ]; then
        rm -rf $1
        svn co http://192.168.15.30/svn/software/wifi/tag/smac-release-tag/$1
        cd $1/ssv6x5x
        script/release
        cd ../..
        svn import $1 http://192.168.15.30/svn/software/QA_tags/Ubuntu-PC/$1 -m "$2 release $1"
        rm -rf $1
        svn co http://192.168.15.30/svn/software/QA_tags/Ubuntu-PC/$1
        cd $1/ssv6x5x
        ./gen-version.sh
        svn commit * -m "$2 release $1"        
        else
        echo "No WSD-XX or NO WSP-XX parameter"
        fi
        echo "Done!"
        else
        echo "No verion parameter!"
        fi
        break;;
    5 ) echo "${release_options[$REPLY-1]} is option";
        if [ "$1" != "" ]; then
        if [ "$2" != "" ]; then
        rm -rf $1
        svn co http://192.168.15.30/svn/software/QA_tags/Ubuntu-PC/$1
        cd $1
        rm -rf hostapd
        cd ..
        svn import $1 http://192.168.15.30/svn/software/project/release/software-mac/$1 -m "$2 release $1"
        rm -rf $1
        svn co http://192.168.15.30/svn/software/project/release/software-mac/$1
        cd $1/ssv6x5x
        ./gen-version.sh
        svn commit * -m "$2 release $1"
        else
        echo "No WSD-XX or NO WSP-XX parameter"
        fi
        echo "Done!"
        else
        echo "No verion parameter!"
        fi
        break;;

    $(( ${#release_options[@]}+1 )) ) echo "Goodbye!"; break;;
    *) echo "Invalid option. Try another one.";continue;;
    esac
done

