#!/bin/bash


readonly BIN_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )
readonly BASE_DIR=$( dirname $BIN_DIR)

readonly INPUT_HTML="$BIN_DIR/html/portal.html"
readonly TEMP_HTML="$BIN_DIR/html/_p.html"
readonly TEMP_GZ="$BIN_DIR/html/_p.gz"
readonly TEMP_BR="$BIN_DIR/html/_p.br"
readonly OUTPUT_DATA="$BASE_DIR/sketch_WMaster/data/portal.htz"

readonly SED_BACKUP_EXT=".sed"


echo "original HTML"
ls -l $INPUT_HTML
sed -E 's/^[[:space:]]*//;s/[[:space:]]*$//;s/\/all.txt/\/\$/' $INPUT_HTML \
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

cp $TEMP_GZ $OUTPUT_DATA

rm "$TEMP_GZ"
rm "$TEMP_BR"
rm "$TEMP_HTML"
