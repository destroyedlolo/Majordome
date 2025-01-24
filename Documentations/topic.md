# .topic
Define **MQTT topic**

## Directives

> [!IMPORTANT]  
> As of v6.0 onward, The syntax has changed for **Rendezvous** and is not any more compatible with previous versions.

### General directives
#### -->> name=
Unique name to identify the topic. If not set, uses the filename.
```
-->> name=Toto
```
#### --> quiet
Removes some trace. This option is useful to avoid logging of very noisy topics.

#### -->> disabled
Message arrival is ignored

### Topics' owns
#### -->> topic=
Topic to listen / publish too. **This field is mandatory.**
```
-->> topic=Temperature/Pool
```
> [!TIP]  
> **%ClientID%** will be replaced by its value.

#### -->> qos=
Quality of service for this topic between **0** and **2**. If outside this range, forced to **0** (which is the default value)
```
-->> qos=2
```

#### -->> store
Store received payload in a SelShared variable with the name of this topic. As example :
```
-->> name=titi
-->> topic=example
-->> store
```
If we receive a value "*toto*" in topic `example`, a SelShared variable "**titi**" is created with value "*toto*".

#### -->> numeric
Store the payload as a numeric value (float).

#### -->> ttl=
Time to live of this SelSharedVar (in seconds). When the topic doesn't receive fresh data during this ttl period, SelSharedVar is unset.
```
-->> ttl=2.5
```

#### -->> default=
Set the default value for this topic. This field must be defined after **name**, **numeric** and **ttl** and is useful only with **store**d topics.
```
-->> default=3.14
```


## Example
File **test.topic**
```
-- test topic
--
-- name of the topic
-- if not set, takes the filename
--->> name=toto
--
-- set the topic (mandatory)
-- if present, %ClientID% will be replaced by Majordome's MQTT client ID
-->> topic=%ClientID%/About
--
-- QOS for this topic between 0 and 2
-- if outside this range, forced to 0 (which is the default value)
-->> qos=0
--
-- Store this topic's content in a SelShared variable with the name of this topic
-->> store
--
-- Store it as a numeric value
-->> numeric
--
-- time to live of this variable (in Seconds)
-->> ttl=2
--
-- set the default value for this topic.
-- must be defined after 'name', 'numeric' and 'ttl'
-- useful only with 'store'd topics
-->> default=3.14
--
-- remove some trace
-- This option is useful to avoid logging of very noisy topics
-->> quiet
--
-- disable this topic
-->> disabled
```

## at Lua side

MQTTTopic are exposed as **MajordomeMQTTTopic** object that supports the following methods :
  - `Publish(value, retain)` to publish a MQTT message with
    - `[string|number] value` value to be published
    - `Boolean retain` does the value to be retained ?
  - `getTopic()` returns the associated MQTT topic
  - `getVal()` returns the last received payload (only if the topic is `stored`). Takes in account also the associated time to live.
  - `Launch()` launches tasks associated to this topic
  - `getContainer()` returns the container (directory) in which this topic as been defined
  - `getName()` returns topic's name
  - `isEnabled()` returns a boolean reflecting if the topic is enabled or not
  - `Enable()` to enable this topic
  - `Disable()` to disable this topic
