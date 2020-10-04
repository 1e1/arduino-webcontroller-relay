#!/bin/bash


readonly BIN_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )
readonly BASE_DIR=$( dirname ${BIN_DIR})

function seal {
file=$1

INPUT_HTML="${BIN_DIR}/html/${file}.html"
TEMP_HTML="${BIN_DIR}/html/_${file}.html"
TEMP_GZ="${BIN_DIR}/html/_${file}.gz"
TEMP_BR="${BIN_DIR}/html/_${file}.br"
OUTPUT_DATA="$BASE_DIR/sketch_WMaster/data/${file}.htz"

SED_BACKUP_EXT=".sed"


echo "original HTML"
ls -l $INPUT_HTML
sed -E 's/^[[:space:]]*//;s/[[:space:]]*$//;s/(\/\$\{u\})\.json/\1/' $INPUT_HTML \
    | tr -d '\r\n' > $TEMP_HTML

# force the modification date to prevent this diff only (date embed into the zip)
touch -t 200404040200 $TEMP_HTML

echo "minify HTML"
ls -l $TEMP_HTML
gzip -c -9 $TEMP_HTML > $TEMP_GZ
brotli -c -Z $TEMP_HTML > $TEMP_BR

#touch -t 200404040200 $TEMP_GZ
#touch -t 200404040200 $TEMP_BR

echo "compressed GZ"
ls -l $TEMP_GZ

echo "compressed BR"
ls -l $TEMP_BR

cp $TEMP_BR $OUTPUT_DATA

rm "$TEMP_GZ"
rm "$TEMP_BR"
rm "$TEMP_HTML"
}


cat <<EOT

portal
######
EOT
seal portal


cat <<EOT

master
######
EOT
seal master
