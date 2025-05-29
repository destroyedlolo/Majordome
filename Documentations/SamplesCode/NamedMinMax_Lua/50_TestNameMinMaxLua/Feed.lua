-- Feed NamedMinMax directly from Lua
--
-- Create the targeted object
-->> need_namedminmax=TestNamedMinMax
--
-->> RunAtStartup

print ""

for i=0,10 do
	TestNamedMinMax:Push("Asc", i)
	TestNamedMinMax:Push("Desc", 100-i)
end

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
