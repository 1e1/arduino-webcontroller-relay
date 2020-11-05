#!/bin/bash


readonly BIN_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )
readonly BASE_DIR=$( dirname $BIN_DIR)

readonly HTML_DIR="$BASE_DIR/web/html/"
readonly MASTER_DIR="$BASE_DIR/sketch_WMaster/"
readonly SLAVE_DIR="$BASE_DIR/sketch_WSlave/"
readonly MASTER_DATA_DIR="$MASTER_DIR/data/"
readonly MASTER_DUMP_DIR="$MASTER_DIR/dump/"

readonly DEFAULT_DNAME="relayboard"
readonly DEFAULT_AP_IP="192.168.4.1"
