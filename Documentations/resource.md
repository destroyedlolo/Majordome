# .resource
Reduces or avoids concurrent access

## Directives

### General directives
### Generals
See [this page](Headers%20and%20Shared%20Directives.md#general-directives).

### Resource' owns
#### -->> limit=

Specify the number of instances allowed : accepted values are between 1 and 254. 
- 255 means unlimited.
- The default is 1.
```
-->> limit=1
```

## Example
File **test.topic**
```
-->> desc=Allow only 1 task at a time
--
--========
-- General
--========
--
-- Specify the name
-- if not set, takes the filename
--->> name=
--
-- Be verbose
--->> verbose
--
-- Remove noise
--->> quiet
--
-- This resource is disabled
-- New tasks can't be launched, but, obviously, existing ones can finish :)
--->> disabled
--
--===========
-- Resource's
--===========
--
-- Specify the number of instances allowed
-- Between 1 and 254. 255 means unlimited.
-- Default is 1.
-->> limit=1
```

## at Lua side

Resources are not currently exposed in Lua. <br>
This is a **deliberate choice** as allowing them to be manipulated *manually* could easily
lead to deadlocks.
