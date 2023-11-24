set -e

sudo docker run --privileged --mount type=bind,source="$(pwd)",target=/balloon luckfoxtech/luckfox_pico:1.0 /bin/bash /balloon/build.sh

zip output.zip output