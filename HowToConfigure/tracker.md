# .tracker
Trackers are a simple state box to track some activities.

As an example, temperature followup to close shutters in order to prevent over heating of room directly exposed to the sun :
- temperature followup is starting at a fixed time (i.e., not needed before 10:00 as the sun is too low), and end up to another one (let's say, after 17:00, it's too late to close shutters).
- during this time window, we are checking if the temperature goes above our target and in such case, shutters are closed and there is no need to continue the followup.

Trackers make easy this kind of simple automation.

## Syntax

Trackers are basically **Lua scripts** and are following the same syntax.

In the header of the script (comment block at the very beginning of the script), each line starting with `-->>` are Majordome's commands.<br>
Consequently, `--->>` are commented out commands (notice the 3 dashes).

## Directives

### Generals

#### -->> name=
Unique name to identify the tracker. If not set, uses the filename.
```
-->> name=toto
```

#### -->> quiet
Remove some trace. This option is useful to avoid logging of very noisy topics.

#### -->> disabled
This tracker starts as disabled : stats change and incoming messages are ignored.

### MQTT topics

#### -->> listen=
Indicates which [Topic](topic.md) to follow.

#### -->> statustopic=
Report status change to the given topic.
(est-il hardcode, ou est-ce un topic ???)
