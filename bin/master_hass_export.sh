#!/bin/bash


readonly SCRIPT_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )
source "$SCRIPT_DIR/_init.sh"


INPUT_HOST="http://esp8266"


while getopts h:p:u: flag
do
    case "${flag}" in
        h) INPUT_HOST=${OPTARG};;
    esac
done


cat <<EOT
# relayboard_master.yaml

EOT


json=`curl \
    --request GET \
    --url "${INPUT_HOST}/api/r" \
    --insecure \
    --silent \
`


relayList=`echo $json | sed 's#[^{]*{"n":"\([^"]*\)","i":\([0-9]*\)[^}]*}[^{]*#\2:\1\n#g'`


while read -r relay
do 
  relayId=${relay%%:*}
  relayName=${relay#*:}

cat <<EOT
- name: "${relayName}"
  resource: "${INPUT_HOST}/api/r/${relayId}"
  platform: rest
  method: put
  body_on: '{"s":1}'
  body_off: '{"s":0}'
  timeout: 30
  verify_ssl: false

EOT
done < <(echo "$relayList")
