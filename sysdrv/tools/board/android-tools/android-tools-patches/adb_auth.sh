#!/bin/sh
VERIFIED=/tmp/.adb_auth_verified

if [ -f "$VERIFIED" ]; then
    echo "success."
    return
fi

for i in $(seq 1 3); do
    read -p "$(hostname -s)'s password: " PASSWD
    if [ "$PASSWD" = "AUTH_PASSWORD" ]; then
        echo "success."
        touch $VERIFIED
        return
    fi

    echo "password incorrect!"
done

false
