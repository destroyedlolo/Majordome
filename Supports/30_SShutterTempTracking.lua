-- Class to handle shutters with temperature tracking

function SShutterTempTracking(aname, atpc, atimer, atemp, astart, astop, alimit)
	local self = SShutter(aname, atpc)

	-- local fields
	local timer = atimer			-- Timer to modify
	local probe = atemp				-- temperature's probe
	local HMonitoringStart = astart	-- start of monitoring
	local HMonitoringStop = astop	-- end of monitoring
	local TemperatureLimit = alimit	-- temperature triggering freshness saving

	-- methods
	local function StopTracking()
		SelLog.log(self.getName() .. " : Fin de la surveillance de la température")
		timer.TaskRemove( LaunchTracking )
		timer.TaskRemove( StopTracking )
		probe.TaskRemove( TemperatureTracking )
	end

	local function TemperatureTracking()
		if probe.get() > TemperatureLimit then
			SelLog.log( self.getName() .. " : " .. probe.get() .. ", les volets se ferment")
			self.My()
			StopTracking()

			timer.TaskAdd( HMonitoringStop, self.Up )
			SelLog.log( self.getName() .. " : Les volets s'ouvriront à " .. HMonitoringStop )
		elseif DEBUG then
			SelLog.log( self.getName() .. " : " .. probe.get() .. ' -> trop basse' )
		end
	end

	local function LaunchTracking()
		SelLog.log(self.getName() .." : Début de la surveillance de la température")
		timer.TaskRemove( LaunchTracking )
		probe.TaskAdd( TemperatureTracking )
	end

	function self.LaunchTrackingAt()
		local cur = atimer.Current()

		if cur < HMonitoringStart then		-- Before monitoring period
			SelLog.log( self.getName() .. " : La surveillance commencera à " .. HMonitoringStart )
			timer.TaskAdd( HMonitoringStart, LaunchTracking )

			SelLog.log( "Et se terminera à " .. HMonitoringStop )
			timer.TaskAdd( HMonitoringStop, StopTracking )

			probe.TaskRemove( TemperatureTracking )
		elseif cur < HMonitoringStop then	-- during
			LaunchTracking()

			SelLog.log( self.getName() .. " : La surveillance se terminera à " .. HMonitoringStop )
			timer.TaskAdd( HMonitoringStop, StopTracking )
		else								-- after
			StopTracking()
		end
	end

	function self.getTimer()
		return timer
	end

	return self
end

