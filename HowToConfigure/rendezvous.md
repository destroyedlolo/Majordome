# .rendezvous
A Rendez-vous triggers an action from another object or from Lua code. 
Typical usage is to synchronize automation within Majordome without having to use intermediate topic.

## Directives

### name=
Unique name to identify the rendez-vous. If not set, uses the filename.
```
name=toto
```

### quiet
Remove some trace. This option is useful to avoid logging of very noisy topics.

### disabled
This topic starts as disabled : incoming messages are ignored.

## Example
File **test.rendezvous**
```
# name of the topic
# Here, commented out, so we take the filename, "test"
# name=

# Don't log incomming messages on this topic
#quiet

# disable this topic
#disabled
```
