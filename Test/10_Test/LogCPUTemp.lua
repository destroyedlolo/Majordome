-- Log CPU temperature
--
-- Name of this script
-- if not set, takes the filename
-- Notez-bien : if used, this directive MUST be defined before any
-- listen directive.
--->> name=Toto
--
-- Indicate the Timer(s) to wait for
-- (more than one "when" can be present)
--->> when=30s
--
-- If set, only one instance is allowed to run
--->> once
--
-- remove some trace
-- This option is useful to avoid logging of very noisy topics
--->> quiet
--
-- Disable this script
--->> disabled
--
-->> need_timer=30s
-->> RunAtStartup

print(_G['30s'])

local f = assert(io.open("/sys/class/thermal/thermal_zone1/temp", "r"))
local t = f:read("*number")
f:close()

SelLog.Log("CPU temperature : ".. t/1000 .. "°C")
