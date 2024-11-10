#!/bin/bash -l


readonly SCRIPT_DIR=$( cd "$( dirname $(realpath "${BASH_SOURCE[0]}") )" >/dev/null && pwd )
source "${SCRIPT_DIR}/_init.sh"


docker run --rm -it \
  -p 8092:80 \
  -v $HTML_DIR:/usr/share/nginx/html:ro \
  --tmpfs /var/lib/nginx \
  --tmpfs /run \
  nginx:alpine
