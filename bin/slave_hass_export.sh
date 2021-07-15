#!/bin/bash


readonly SCRIPT_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )
source "$SCRIPT_DIR/_init.sh"


INPUT_HOST="http://relayboard"


while getopts h:p:u: flag
do
    case "${flag}" in
        h) INPUT_HOST=${OPTARG};;
    esac
done


cat <<EOT
# relayboard_slave.yaml


- platform: command_line
  switches:
  
EOT


text=`curl \
    --request GET \
    --url "${INPUT_HOST}/$" \
    --insecure \
    --silent \
`


relayList=`echo "$text" | awk '{ print $2 }'`


while read -r relayId
do 

cat <<EOT
    wc_r${relayId}:
      command_on: "/usr/bin/curl -X GET ${INPUT_HOST}/w/${relayId}/1"
      command_off: "/usr/bin/curl -X GET ${INPUT_HOST}/w/${relayId}/0"
      command_state: "/usr/bin/curl -X GET ${INPUT_HOST}/r/${relayId}"
      value_template: '{{ value.split()[0] == "1" }}'
      friendly_name: "Relay #${relayId}"

EOT
done < <(echo "$relayList")
