-->> desc=Display MinMax's content (debugging purpose)
--
-- Samples periode
-->> when=Periode
--
-- MinMax to investigate
-->> need_minmax=Aggregator
--
-- remove some trace
--->> quiet
--
-- Disable this service
--->> disabled
--

print ""

--	Iterate against stored values
print ""
print( "Number of samples :", Aggregator:getSamplesNumber() )
print( "Min value :", Aggregator:getMin() )
print( "Max value :", Aggregator:getMax() )
print( "Max Average :", Aggregator:getAverage() )

-- Clear storage : restart a new series
Aggregator:Clear()
