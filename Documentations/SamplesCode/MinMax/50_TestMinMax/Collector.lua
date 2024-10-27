-- This task with collect minmax data every 1 minute.
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
-- Indicate the Timer(s) to wait for
-- (more than one "when" can be present)
-->> when=10s
--
-- Create this rendez-vous object
-->> need_minmax=TestMinMax
--
-- remove some trace
-- This option is useful to avoid logging of very noisy topics
--->> quiet
--
-- Disable this script
--->> disabled
--

-- Get MinMax values
print ""
print( "Number of samples :", TestMinMax:getSamplesNumber() )
print( "Min value :", TestMinMax:getMin() )
print( "Max value :", TestMinMax:getMax() )
print( "Max Average :", TestMinMax:getAverage() )

-- Clear storage : restart a new series
TestMinMax:Clear()
