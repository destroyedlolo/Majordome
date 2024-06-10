-- Tasks that will launch bip one
--
-- In the header of the script (comment block at the very beginning of
-- the script), each line starting with -->> are Majordome's commands.
-- Consequently, '--->>' are commented out commands.
--
-- Name of this script
-- if not set, takes the filename
-- Notez-bien : if used, this directive MUST be defined before any
-- listen directive.
--->> name=Toto
--
-->> need_task=bip
--
-- remove some trace
-- This option is useful to avoid logging of very noisy topics
--->> quiet
--
-- Disable this script
--->> disabled
--
-- Start at Majordome startup
-->> RunAtStartup

SelLog.Log("No 'bip' expected before this message")
SelLog.Log("(otherwise, 'require' doesn't apply to 'RunAtStartup')")

SelLog.Log("----")
SelLog.Log("Launching bip with variable unset")
SelLog.Log("(Expected not to work)")

bip:Launch()

SelLog.Log("----")
SelLog.Log("Launching bip with variable set")
SelLog.Log("(Expected to work)")
SelSharedVar.Set("Test", "My value")

bip:Launch()
