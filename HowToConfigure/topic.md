# .topic
Define **MQTT topic**

## Directives

### name=
Unique name to identify the topic. If not set, uses the filename.
```
name=toto
```

### topic=
Topic to listen / publish too. **This field is mandatory.**
```
topic=Temperature/Pool
```

### qos=
Quality of service for this topic between **0** and **2**. If outside this range, forced to **0** (which is the default value)
```
qos=2
```

### store
Store received payload in a SelShared variable with the name of this topic. As example :
```
name=titi
topic=example
store
```
If we receive a value "*toto*" in topic `example`, a SelShared variable "**titi**" is created with value "*toto*".

### numeric
Store the payload as a numeric value (float).

### ttl=
Time to live of this SelShared (in seconds). When the topic doesn't receive fresh data during this ttl period, SelShared is unset.
```
ttl=2.5
```

### default=
Set the default value for this topic. This field must be defined after **name**, **numeric** and **ttl** and is useful only with **store**d topics.
```
default=3.14
```

### quiet
Remove some trace. This option is useful to avoid logging of very noisy topics.

### disabled
This topic starts as disabled : incoming messages are ignored.

## Example
File **test.topic**
```
# name of the topic
# Here, commented out, so we take the filename, "test"
# name=

# set the topic (mandatory)
# if present, %ClientID% will be replaced by Majordome's MQTT client ID
topic=%ClientID%/myValue

# QOS for this topic between 0 and 2
# if outside this range, forced to 0 (which is the default value)
qos=0

# Store this topic's content in a SelShared variable with the name of this topic
store

# Store it as a numeric value
numeric

# time to live of this variable (in Seconds)
ttl=2

# set the default value for this topic.
#default=3.14

# Don't log incomming messages on this topic
#quiet

# disable this topic
#disabled
```
