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
-->> when=30s
--
-- Create NamedMinMax object
-->> need_namedminmax=TestNamedMinMax
--
-- remove some trace
-- This option is useful to avoid logging of very noisy topics
--->> quiet
--
-- Disable this script
--->> disabled
--

print ""

--	Iterate against stored values
for _,v in ipairs( table.pack(TestNamedMinMax:FiguresNames()) ) do

	print(v)
	print "---------"

	print( "Number of samples :", TestNamedMinMax:getSamplesNumber(v) )
	print( "Min value :", TestNamedMinMax:getMin(v) )
	print( "Max value :", TestNamedMinMax:getMax(v) )
	print( "Max Average :", TestNamedMinMax:getAverage(v) )

	-- Clear storage : restart a new series
	TestNamedMinMax:Clear(v)
end
