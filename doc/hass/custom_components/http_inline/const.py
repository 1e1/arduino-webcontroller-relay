"""Const for the http_inline integration."""


DOMAIN = "HTTP_INLINE"
PLATFORMS = ["switch"]

CONF_PATH_PATTERN_READ = "path_read"
CONF_PATH_PATTERN_WRITE = "path_write"
CONF_RELAYS = "relays"
CONF_RELAY_ID_MIN = "first_relay_id"
CONF_RELAY_ID_MAX = "last_relay_id"

DEFAULT_HOST = "http://webrelay.local"
DEFAULT_NAME = "http_inline switch"
DEFAULT_PATH_PATTERN_READ = '/r/{relay_id}'
DEFAULT_PATH_PATTERN_WRITE = '/w/{relay_id}/{state}'
DEFAULT_RELAY_ID_MIN = '0'
DEFAULT_RELAY_ID_MAX = '0'
