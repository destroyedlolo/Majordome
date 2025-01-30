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
-->> need_topic=Test
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

Test:Launch()

