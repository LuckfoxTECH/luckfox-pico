find ./www-rkipc -name '3rdpartylicenses.txt' | xargs rm
find ./www-rkipc -name '*es5*' | xargs rm

find ./www-rkipc -name '*.js' | xargs gzip -9
find ./www-rkipc -name '*.png' | xargs gzip -9
find ./www-rkipc -name '*.css' | xargs gzip -9
find ./www-rkipc -name '*.json' | xargs gzip -9
