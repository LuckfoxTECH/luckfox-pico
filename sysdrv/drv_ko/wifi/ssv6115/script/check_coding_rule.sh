#!/bin/bash

ROOT_PATH=`pwd`

#1.禁止使用printk(), 需使用SSV_LOG_XXX()
if find $ROOT_PATH -name "*.c" | xargs grep -q "printk"; then
    echo "!!!!plese use the SSV_LOG_XXX() replace the printk"
    find $ROOT_PATH -name "*.c" | xargs grep "printk"
    exit 1
fi
