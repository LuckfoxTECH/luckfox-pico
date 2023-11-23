set -e

sudo docker run --privileged --mount type=bind,source="$(pwd)",target=/balloon luckfoxtech/luckfox_pico:latest /bin/bash /balloon/build.sh