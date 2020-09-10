# WrightSlave Relay

A single controller connected by Web or USB.

![landscape](./doc/hardware.jpg)


## Setup

Edit `./sketch_WSlave/config.h`


## Commands

Standard no-REST routes:

- **$**: `/$`
  full status list of all pins if `#define MODE_VERBOSE MODE_VERBOSE_LIST` (or `MODE_VERBOSE_ALL` by default)

- **r**ead: `/r/{relay_id}`
  read relay state

- **w**rite: `/w/{relay_id}/{value}`
  set a state (0-1) to this relay
  (set a pin to OUTPUT mode)

- **m**ap: `/m/{relay_id}/{pin_id}`
  map a relay to a pin controller

- N**o**: `/c/{relay_id}`
  set a relay to NC mode

- N**c**: `/o/{relay_id}`
  set a relay to NO mode

- save (**!**): `/!`
  save the wiring into EEPROM, not the values ON/OFF if `#define DATA_STORAGE != DATA_STORAGE_NONE`

- reset (**~**): `/~`
  reset the board  if `#define ACL_ALLOW ACL_ALLOW_RESET` (or `ACL_ALLOW_RESET` by default)

Read the ![swagger](./doc/swagger.yml)

### Example for mapping the relay #7 on the pin #42

#### HTTP

Go to `http://webrelay.local/m/7/42`

#### Serial

Send `/m/7/42`


## webApp

`#define MODE_SERIAL MODE_SERIAL_ETHERNET` (or `MODE_SERIAL_ALL` by default)

The Arduino boots on the DHCP.
If `#define MODE_VERBOSE MODE_VERBOSE_WEBAPP` (or `MODE_VERBOSE_ALL` by default),
open a bowser on `http://{ip}`.


## USB

`#define MODE_SERIAL MODE_SERIAL_USB` (or `MODE_SERIAL_ALL` by default)

If `#define MODE_VERBOSE MODE_VERBOSE_HELP` (or `MODE_VERBOSE_ALL` by default),
write anything and the read the help


## dependancies

### software

- EthernetBonjour if `MODE_BONJOUR` != `MODE_BONJOUR_NONE`


### integrations

#### Home Assistant (hass)

![hass](./doc/hass/preview.png)

##### UI Wizard setup

If the component is not embed in your HASS version,
install `http_inline` from `./doc/hass/custom_compoenents/http_inline` to `/config/custom_compoenents/http_inline`,
then use the wizard.


##### CLI install

###### Custom component

Add the following lines into `configuration.yaml`,
and apply this template:

```yaml
switch:
  - platform: http_inline
    host: http://{host}
    relays:
      {relayId}: {friendlyName}
```

And customize:
- `{host}`: WebController IP or host
- `{relayId}`: WebController ID
- `{friendlyName}`: a free text

Example:

```yaml
switch:
  - platform: http_inline
    host: http://web
    relays:
      0: Fan Office
      1: Light Desk
      2: Light Kitchen
      3: Light Living room
      4: Light Bedroom 1
      5: Light Bedroom 2
      6: Light Garage
      7: Light Garden
```

###### Official

Add the following lines into `configuration.yaml`,
and apply this template:

```yaml
switch:
  - platform: command_line
    switches:
      {sluggedEntityId}:
        command_on: "/usr/bin/curl -X GET http://{host}/w/{relayId}/1"
        command_off: "/usr/bin/curl -X GET http://{host}/w/{relayId}/0"
        command_state: "/usr/bin/curl -X GET http://{host}/r/{relayId}"
        value_template: '{{ value.split()[0] == "1" }}'
        friendly_name: {friendlyName}
```

And customize:
- `{sluggedEntityId}`: HASS entity ID
- `{host}`: WebController IP or host
- `{relayId}`: WebController ID
- `{friendlyName}`: a free text

Example:

```yaml
switch:
  - platform: command_line
    switches:
      r0:
        command_on: "/usr/bin/curl -X GET http://webrelay.local/w/0/1"
        command_off: "/usr/bin/curl -X GET http://webrelay.local/w/0/0"
        command_state: "/usr/bin/curl -X GET http://webrelay.local/r/0"
        value_template: '{{ value.split()[0] == "1" }}'
        friendly_name: "Relay #0"
      r42:
        command_on: "/usr/bin/curl -X GET http://webrelay.local/w/42/1"
        command_off: "/usr/bin/curl -X GET http://webrelay.local/w/42/0"
        command_state: "/usr/bin/curl -X GET http://webrelay.local/r/42"
        value_template: '{{ value.split()[0] == "1" }}'
        friendly_name: "Relay #42"
```

#### NodeRed

![hass](./doc/nodered/preview.png)

Import the `./doc/nodered/flows_subFlowAndTest.json` (or `flow_subFlowOnly.json`)


### tools

#### custom HTML

- edit ./web/html/index.html
- export to ./sketch_WSlave/_webApp.h by `./web/html2h.h`
- run `./web/docker-compose up` for testing

#### docker-compose

```bash
$ ./web
$ docker-compose up
```

- test custom HTML on http://{docker-machine}:8080
- test Home Assistant on http://{docker-machine}:8123
- test NodeRed on http://{docker-machine}:1880


#### Suggestions

TODO: read states from calendar
