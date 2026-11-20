This use case translates the `STATE` topic published by a **Tasmota** powered device into the topics expected by
[Domotik](https://github.com/destroyedlolo/Domestik) host monitoring.  
To match the Domestik naming convention, STATE messages must be published to `Machines/+/tele/STATE`, where **+** corresponds
to the device's hostname.

> [!IMPORTANT]
> Since this STATE topic differs from Tasmota's default structure, you need to customise it in the device's
>  MQTT configuration page with the following settings:
>  - **Topic**: `<device_hostname>` (your device's hostname)
>  - **Full Topic**: `Machines/%topic%/%prefix%/`
