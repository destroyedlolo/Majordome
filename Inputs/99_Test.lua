if TEST then

	local function ITest()	-- Test call to test inheritance
		local self = MQTTinput('Test', 'Test', Topic2Number)

		-- methods
		local function arrived() -- Tasks called when Test is updated
			print('arrived', SelShared.get('Test'))
		end

			-- It's only a test : it has nothing to do in input
		local function tempchanged() -- When TExterieur is changed
			print("TExt", TExterieur.get() or 'not set')
		end

		-- initialiser
		self.TaskAdd( arrived )
		TExterieur.TaskAdd( tempchanged )
	end

	tst = ITest()


		-- Timer test
	function bip1()
		print 'bip 1'
	end

	function bip2()
		print 'bip 2'
	end

	function bip3()
		print 'bip 3'
	end

	local dt = os.date("*t")
	local cur = dt.hour + dt.min/100
	local t = DEC2DMS(DMS2DEC(cur) + DMS2DEC(0.01))	-- Dans 1'

	Timer.TaskAdd( t, bip1 );
	
	t = DEC2DMS(DMS2DEC(cur) + DMS2DEC(0.02))	-- Dans 2'

	Timer.TaskAdd( t, bip2 );
	Timer.TaskAdd( t, bip3 );
end
