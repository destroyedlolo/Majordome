local function ITest()	-- Test call to test inheritance
	local self = MQTTinputs('Test', 'Test', Topic2Number)

	-- methods
	local function arrived() -- Tasks called when Test is updated
		print('arrived', SelShared.get('Test'))
	end

		-- It's only a test : it has nothing to do in input
	local function tempchanged() -- When TExterieur is changed
		print("TExt", TExterieur.get() or 'not set')
	end

	-- initialiser
	self.TasksAdd( arrived )
	TExterieur.TasksAdd( tempchanged )
end

tst = ITest()
