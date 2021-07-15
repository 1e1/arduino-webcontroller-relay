# 🚥 Wright Relay


## Slave sketch


### integrations

#### Home Assistant (hass)

![hass](./hass/preview.png)


##### CLI install

###### Master integration

Get the Relayboard configuration with the tool: 
```bash
$ ./bin/master_hass_export.sh -h https://webrelay.local > ./relayboard_master.yaml
```

Then move this file to the Hass config folder. 

Example with the required lines:


- `$HASS_CONFIG/configuration.yaml`
```yaml
homeassistant:
  packages: !include_dir_named integrations
```
- `$HASS_CONFIG/integrations/switch.yaml`
```yaml
switch: !include_dir_merge_list ../entities/switches
```
- `$HASS_CONFIG/entities/switches/relayboard_master.yaml` exported from the tool
```yaml
# relayboard_master.yaml

- name: "gateway"
  resource: "https://192.168.10.1/api/r/0"
  platform: rest
  method: put
  body_on: '{"s":1}'
  body_off: '{"s":0}'
  timeout: 30
  verify_ssl: false

- name: "living room"
  resource: "https://192.168.10.1/api/r/1"
  platform: rest
  method: put
  body_on: '{"s":1}'
  body_off: '{"s":0}'
  timeout: 30
  verify_ssl: false

```



###### Slave Only

Get the Relayboard configuration with the tool: 
```bash
$ ./bin/master_hass_export.sh -h https://relayboard.local > ./relayboard_slave.yaml
```

You should edit the exported file and rename the relay names at `friendly_name` ("Relay ##" by default).
Then move this file to the Hass config folder. 

Example with the required lines:


- `$HASS_CONFIG/configuration.yaml`
```yaml
homeassistant:
  packages: !include_dir_named integrations
```
- `$HASS_CONFIG/integrations/switch.yaml`
```yaml
switch: !include_dir_merge_list ../entities/switches
```
- `$HASS_CONFIG/entities/switches/relayboard_slave.yaml` exported from the tool
```yaml
# relayboard_slave.yaml


- platform: command_line
  switches:
    wc_r1:
      command_on: "/usr/bin/curl -X GET http://192.168.10.1/w/1/1"
      command_off: "/usr/bin/curl -X GET http://192.168.10.1/w/1/0"
      command_state: "/usr/bin/curl -X GET http://192.168.10.1/r/1"
      value_template: '{{ value.split()[0] == "1" }}'
      friendly_name: "Relay #1"

    wc_r0:
      command_on: "/usr/bin/curl -X GET http://192.168.10.1/w/0/1"
      command_off: "/usr/bin/curl -X GET http://192.168.10.1/w/0/0"
      command_state: "/usr/bin/curl -X GET http://192.168.10.1/r/0"
      value_template: '{{ value.split()[0] == "1" }}'
      friendly_name: "Relay #0"

```


##### NodeRed

![hass](./nodered/preview.png)

Import the `./nodered/flows_subFlowAndTest.json` (or `flow_subFlowOnly.json`)


## Master sketch


## tools

### custom HTML

- edit ./web/html/slave.html
- export to ./sketch_WSlave/webApp-generated-*.h by `./bin/slave_html_generate.sh`
- export to ./sketch_WMaster/data/* by `./bin/master_html_generate.sh`
- export to ./sketch_WMaster/certificate-generated.h/* by `./bin/master_certificate_generate.sh`
- run `./web/docker-compose up` for testing

### docker-compose

```bash
$ ./web
$ docker-compose up
```

- test custom HTML on http://{docker-machine}:8080
- test Home Assistant on http://{docker-machine}:8123
- test NodeRed on http://{docker-machine}:1880

### help

You can watch the communication between an Arduino Mega slave and a ESP master with SerialTools.
Use a separate Wemos D1 mini and a standard Arduino Mega. 

![app SerialTools](./serialtools.png)


A video explains how to upload the sketches on an Arduino Mega ESP8266
[![HOWTO on YouTube](https://i.ytimg.com/vi_webp/7OckOeyoso8/maxresdefault.webp)](https://www.youtube.com/watch?v=7OckOeyoso8)

Backup a configuration into `./sketch_WMaster/dump/` with `./bin/master_config_download.sh`, 
then upload it on a fresh install with `./bin/master_config_upload.sh`.
