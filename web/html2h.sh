#!/bin/bash


readonly BIN_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )
readonly BASE_DIR=$( dirname $BIN_DIR)

readonly INPUT_HTML="$BIN_DIR/html/index.html"
readonly TEMP_HTML="$BIN_DIR/html/_.html"
readonly TEMP_GZ="$BIN_DIR/html/_.gz"
readonly TEMP_BR="$BIN_DIR/html/_.br"
readonly OUTPUT_GZ_H="$BASE_DIR/sketch_WSlave/webApp-generated-gzip.h"
readonly OUTPUT_BR_H="$BASE_DIR/sketch_WSlave/webApp-generated-brotli.h"

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

echo "write .h"
echo "" >  $OUTPUT_GZ_H
########################################

cat <<EOT >> $OUTPUT_GZ_H
#ifndef _webApp_H_
#define _webApp_H_

#define WEBPAGE { \\
EOT

hexdump -v -e '/1 "0x%02X, "' $TEMP_GZ >> $OUTPUT_GZ_H
sed -i$SED_BACKUP_EXT '$s/, $/ \}/' $OUTPUT_GZ_H

cat <<EOT >> $OUTPUT_GZ_H

#endif // _webApp_H_
EOT

########################################
ls -l $OUTPUT_GZ_H

echo "write .h"
echo "" >  $OUTPUT_BR_H
########################################

cat <<EOT >> $OUTPUT_BR_H
#ifndef _webApp_H_
#define _webApp_H_

#define WEBPAGE { \\
EOT

hexdump -v -e '/1 "0x%02X, "' $TEMP_BR >> $OUTPUT_BR_H
sed -i$SED_BACKUP_EXT '$s/, $/ \}/' $OUTPUT_BR_H

cat <<EOT >> $OUTPUT_BR_H

#endif // _webApp_H_
EOT

########################################
ls -l $OUTPUT_BR_H

rm "$OUTPUT_GZ_H$SED_BACKUP_EXT"
rm "$OUTPUT_BR_H$SED_BACKUP_EXT"
rm "$TEMP_GZ"
rm "$TEMP_BR"
# rm "$TEMP_HTML"
