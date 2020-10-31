#!/bin/bash


readonly BIN_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )
readonly BASE_DIR=$( dirname ${BIN_DIR})


readonly DEFAULT_HOST="webrelay"
readonly DEFAULT_BASEURL="/cfg/"

readonly OUTPUT_DIR="$BASE_DIR/sketch_WMaster/dump/"

INPUT_HOST="esp8266"
INPUT_USER=""
INPUT_PASSWORD=""


while getopts h:p:u: flag
do
    case "${flag}" in
        h) INPUT_HOST=${OPTARG};;
        p) INPUT_PASSWORD=${OPTARG};;
        u) INPUT_USER=${OPTARG};;
    esac
done

function download_config {
filename=$(basename -- "$1")
extension="${filename##*.}"
filename="${filename%.*}"

curl \
    --request GET \
    --url "https://${INPUT_HOST}/cfg/${filename}" \
    --insecure \
    --basic \
    --user "${INPUT_USER}:${INPUT_PASSWORD}" \
    --silent \
    > "${OUTPUT_DIR}/$1"

}


cat <<EOT

download global.json
######
EOT
download_config g.json


cat <<EOT

download wifi.json
######
EOT
download_config w.json


cat <<EOT

download relay.json
######
EOT
download_config r.json


cat <<EOT

download wiring.json
######
EOT
download_config p.json

ls -lh $OUTPUT_DIR
