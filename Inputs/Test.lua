local function CTest()	-- Test call to test inheritance
	local self = MQTTinputs('Test', 'Test', Topic2Number)

	-- methods
	local function arrived() -- Tasks called when Test is updated
		print('arrived', SelShared.get('Test'))
	end

	-- initialiser
	self.TasksAdd( arrived );
end

tst = CTest()
