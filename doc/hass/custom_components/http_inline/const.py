"""Const for the http_inline integration."""


DOMAIN = "http_inline"
PLATFORMS = ["switch"]

CONF_PATH_PATTERN_READ = "path_read"
CONF_PATH_PATTERN_WRITE = "path_write"
CONF_SWITCHES = "switches"
CONF_NB_SWITCHES = "nb_switches"
CONF_SWITCH_NAMES = "switch_names"
CONF_SWITCH_I_NAME_PATTERN = "switch_name_{}"

DEFAULT_HOST = "http://webrelay.local"
DEFAULT_NAME = "http_inline switch"
DEFAULT_PATH_PATTERN_READ = "/r/{switch_id}"
DEFAULT_PATH_PATTERN_WRITE = "/w/{switch_id}/{state}"
DEFAULT_NB_SWITCHS = "0"
