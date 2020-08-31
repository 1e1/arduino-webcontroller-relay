"""Config flow to configure the WebController-Relay integration."""
import logging

import requests
import voluptuous as vol

from homeassistant import config_entries
from homeassistant.const import CONF_NAME, CONF_HOST, HTTP_OK
from homeassistant.helpers import config_validation as cv

from .const import ( # pylint: disable=unused-import
    DOMAIN, 
    CONF_PATH_PATTERN_READ, 
    CONF_PATH_PATTERN_WRITE,
    CONF_RELAY_ID_MIN,
    CONF_RELAY_ID_MAX,
    DEFAULT_HOST, 
    DEFAULT_NAME, 
    DEFAULT_PATH_PATTERN_READ, 
    DEFAULT_PATH_PATTERN_WRITE,
    DEFAULT_RELAY_ID_MIN,
    DEFAULT_RELAY_ID_MAX,
)

_LOGGER = logging.getLogger(__name__)


class HttpInlineFlowHandler(config_entries.ConfigFlow, domain=DOMAIN):
    """Handle a config flow."""

    VERSION = 1
    CONNECTION_CLASS = config_entries.CONN_CLASS_LOCAL_PUSH

    def _data_schema(self, user_input=None):
        if user_input is None:
            user_input = {}

        return vol.Schema(
            {
                vol.Required(CONF_HOST, default=user_input.get(CONF_HOST, DEFAULT_HOST)): str, #cv.url,
                vol.Optional(CONF_NAME, default=user_input.get(CONF_NAME, DEFAULT_NAME)): str, #cv.string,
                vol.Optional(CONF_PATH_PATTERN_READ, default=user_input.get(CONF_PATH_PATTERN_READ, DEFAULT_PATH_PATTERN_READ)): str, #cv.string,
                vol.Optional(CONF_PATH_PATTERN_WRITE, default=user_input.get(CONF_PATH_PATTERN_WRITE, DEFAULT_PATH_PATTERN_WRITE)): str, #cv.string,
                vol.Optional(CONF_RELAY_ID_MIN, default=user_input.get(CONF_RELAY_ID_MIN, DEFAULT_RELAY_ID_MIN)): int, #cv.positive_int,
                vol.Optional(CONF_RELAY_ID_MAX, default=user_input.get(CONF_RELAY_ID_MAX, DEFAULT_RELAY_ID_MAX)): int, #cv.positive_int,
            }
        )

    def _sanitize_user_input(self, user_input):
        host = user_input[CONF_HOST]
        name = user_input.get(CONF_NAME, user_input[CONF_HOST])
        path_read = user_input.get(CONF_PATH_PATTERN_READ, DEFAULT_PATH_PATTERN_READ)
        path_write = user_input.get(CONF_PATH_PATTERN_WRITE, DEFAULT_PATH_PATTERN_WRITE)
        relay_id_min = user_input.get(CONF_RELAY_ID_MIN, DEFAULT_RELAY_ID_MIN)
        relay_id_max = user_input.get(CONF_RELAY_ID_MAX, DEFAULT_RELAY_ID_MAX)

        return {
            CONF_HOST: host, 
            CONF_NAME: name, 
            CONF_PATH_PATTERN_READ: path_read, 
            CONF_PATH_PATTERN_WRITE: path_write, 
            CONF_RELAY_ID_MIN: relay_id_min, 
            CONF_RELAY_ID_MAX: relay_id_max,
        }

    def _show_setup_form(self, user_input=None, errors=None):
        """Show the setup form to the user."""

        return self.async_show_form(
            step_id="user",
            data_schema=self._data_schema(user_input),
            errors=errors or {},
        )

    async def async_step_user(self, user_input=None):
        """Handle a flow initiated by the user."""
        errors = {}

        if user_input is None:
            return self._show_setup_form(user_input, errors)

        await self.async_set_unique_id(user_input[CONF_HOST])

        return await self.async_step_link(user_input)

    async def async_step_link(self, user_input=None):
        """Handle a flow initiated by the user."""
        errors = {}

        try:
            request = requests.get(user_input[CONF_HOST], timeout=10)
            if request.status_code == HTTP_OK:
                return await self.async_step_import(user_input)

            _LOGGER.error("Server error {}".format(request.status_code))
            errors[CONF_HOST] = "Server error"

        except requests.exceptions.MissingSchema:
            _LOGGER.error("Missing host or schema in configuration. Add http:// to your URL")
            errors[CONF_HOST] = "Missing host or schema in configuration. Add http:// to your URL"
            
        except requests.exceptions.ConnectionError:
            _LOGGER.error("No route to device at {}".format(user_input[CONF_HOST]))
            errors[CONF_HOST] = "No route to device"

        except Exception:  # pylint: disable=broad-except
            _LOGGER.exception(
                "Unknown error connecting with server at {}".format(user_input[CONF_HOST])
            )
            errors["base"] = "unknown"

        return self.async_show_form(step_id="user", errors=errors)

    async def async_step_import(self, user_input=None):
        """Import a config entry."""
        data = self._sanitize_user_input(user_input)

        self.hass.data[DOMAIN] = data
        
        await self.async_set_unique_id(data[CONF_HOST], raise_on_progress=False)
        return self.async_create_entry(title=data[CONF_NAME], data=data)
