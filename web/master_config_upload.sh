#!/bin/bash


readonly BIN_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )
readonly BASE_DIR=$( dirname ${BIN_DIR})


readonly DEFAULT_HOST="webrelay"
readonly DEFAULT_BASEURL="/cfg/"

readonly INPUT_DIR="$BASE_DIR/sketch_WMaster/dump/"

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

function upload_config {
filename=$(basename -- "$1")
extension="${filename##*.}"
filename="${filename%.*}"

curl \
    --request POST \
    --url "https://${INPUT_HOST}/cfg/${filename}" \
    --insecure \
    --header 'Content-Type: application/json' \
    --data @"${INPUT_DIR}/$1" \
    --silent \

}


ls -lh $INPUT_DIR


cat <<EOT

upload global.json
######
EOT
upload_config g.json


cat <<EOT

upload wifi.json
######
EOT
upload_config w.json


cat <<EOT

upload relay.json
######
EOT
upload_config r.json


cat <<EOT

upload wiring.json
######
EOT
upload_config p.json


cat <<EOT

reboot
######
EOT
curl \
    --request DELETE \
    --url "https://${INPUT_HOST}/cfg/reboot" \
    --insecure \
    --basic \
    --user "${INPUT_USER}:${INPUT_PASSWORD}" \
    --silent 
