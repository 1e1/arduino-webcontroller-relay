"""Support for WebController-Relay devices"""
import asyncio
import logging

import requests
import voluptuous as vol

from homeassistant.config_entries import SOURCE_IMPORT, ConfigEntry
from homeassistant.const import CONF_NAME, CONF_HOST, CONF_ID
from homeassistant.helpers import config_validation as cv
from homeassistant.helpers.typing import ConfigType, HomeAssistantType

from .const import (
    DOMAIN, 
    CONF_PATH_PATTERN_READ, 
    CONF_PATH_PATTERN_WRITE, 
    CONF_RELAYS, 
    DEFAULT_NAME, 
    DEFAULT_PATH_PATTERN_READ, 
    DEFAULT_PATH_PATTERN_WRITE,
)

_LOGGER = logging.getLogger(__name__)

SUPPORTED_PLATFORMS = [
    "switch",
]

_SWITCHES_SCHEMA = vol.Schema(
    {
        vol.Required(CONF_ID): cv.positive_int,
        vol.Optional(CONF_NAME, default=DEFAULT_NAME): cv.string,
    }
)

HTTP_INLINE_SCHEMA = vol.Schema(
    {
        vol.Required(CONF_HOST): cv.url,
        vol.Optional(CONF_NAME, default=DEFAULT_NAME): cv.string,
        vol.Optional(CONF_PATH_PATTERN_READ, default=DEFAULT_PATH_PATTERN_READ): cv.string,
        vol.Optional(CONF_PATH_PATTERN_WRITE, default=DEFAULT_PATH_PATTERN_WRITE): cv.string,
        vol.Required(CONF_RELAYS): _SWITCHES_SCHEMA,
    }
)

CONFIG_SCHEMA = vol.Schema(
    {DOMAIN: vol.Schema(vol.All(cv.ensure_list, [HTTP_INLINE_SCHEMA]))},
    extra=vol.ALLOW_EXTRA,
)


async def async_setup(hass: HomeAssistantType, config: ConfigType) -> bool:
    """Set up the WebController-Relay component."""
    return True

async def async_setup_entry(hass: HomeAssistantType, entry: ConfigEntry):
    """Set up the WebController-Relay component."""
    _LOGGER.error(entry)

    for platform in SUPPORTED_PLATFORMS:
        hass.async_create_task(
            hass.config_entries.async_forward_entry_setup(entry, platform)
        )

    return True
