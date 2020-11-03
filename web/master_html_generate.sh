#!/bin/bash


readonly BIN_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )
readonly BASE_DIR=$( dirname ${BIN_DIR})

OUTPUT_FORMAT="br"
OUTPUT_DIR="$BASE_DIR/sketch_WMaster/data"


while getopts cf: flag
do
    case "${flag}" in
        c) rm -f $OUTPUT_DIR/*;;
        f) OUTPUT_FORMAT=${OPTARG};;
    esac
done


function seal {
filename=$(basename -- "$1")
extension="${filename##*.}"
filename="${filename%.*}"

INPUT_HTML="${BIN_DIR}/html/${filename}.${extension}"
TEMP_HTML="${BIN_DIR}/html/_${filename}.${extension}"
TEMP_GZ="${BIN_DIR}/html/_${filename}.gz"
TEMP_BR="${BIN_DIR}/html/_${filename}.br"
TEMP_EXT="${BIN_DIR}/html/_${filename}.${OUTPUT_FORMAT}"
OUTPUT_DATA="${OUTPUT_DIR}/${filename}.${OUTPUT_FORMAT}"

SED_BACKUP_EXT=".sed"


echo "original HTML"
ls -l $INPUT_HTML
sed -E 's/^[[:space:]]*//;s/[[:space:]]*$//;s/(\$\{.\})\.json/\1/' $INPUT_HTML \
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

cp $TEMP_EXT $OUTPUT_DATA

rm "$TEMP_GZ"
rm "$TEMP_BR"
#rm "$TEMP_HTML"
}


cat <<EOT

portal
######
EOT
seal portal.html


cat <<EOT

master
######
EOT
seal master.html

mv "$BASE_DIR/sketch_WMaster/data/master.${OUTPUT_FORMAT}" "$BASE_DIR/sketch_WMaster/data/index.${OUTPUT_FORMAT}"
