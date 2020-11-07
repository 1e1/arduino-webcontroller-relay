#!/bin/bash


readonly SCRIPT_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )
source "$SCRIPT_DIR/_init.sh"


INPUT_HOST="http://esp8266"
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

echo "GET ${INPUT_HOST}/cfg/${filename}"

curl \
    --request GET \
    --url "${INPUT_HOST}/cfg/${filename}" \
    --insecure \
    --basic \
    --user "${INPUT_USER}:${INPUT_PASSWORD}" \
    --silent \
    > "${MASTER_DUMP_DIR}/$1"

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


echo ls -lh $MASTER_DUMP_DIR
ls -lh $MASTER_DUMP_DIR
