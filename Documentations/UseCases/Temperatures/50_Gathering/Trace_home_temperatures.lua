-->> desc=Display MinMax's content (debugging purpose)
--
-- Samples periode
-->> when=Periode
--
-- NamedMinMax to investigate
-->> need_namedminmax=home_temperatures
--
-- remove some trace
--->> quiet
--
-- Disable this service
--->> disabled
--

print ""

--	Iterate against stored values
for _,v in ipairs( table.pack(home_temperatures:FiguresNames()) ) do

	print(v)
	print "---------"

	print( "Number of samples :", home_temperatures:getSamplesNumber(v) )
	print( "Min value :", home_temperatures:getMin(v) )
	print( "Max value :", home_temperatures:getMax(v) )
	print( "Max Average :", home_temperatures:getAverage(v) )

	-- Clear storage : restart a new series
	home_temperatures:Clear(v)
end
