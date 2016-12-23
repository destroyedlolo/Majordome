-- Class to handle shutters with temperature survey

function SShutterTempSurvey(aname, atpc, atimer, atemp, astart, astop, alimit)
	local self = SShutter(aname, atpc)

	-- local fields
	local timer = atimer			-- Timer to modify
	local probe = atemp				-- temperature's probe
	local HMonitoringStart = astart	-- start of monitoring
	local HMonitoringStop = astop	-- end of monitoring
	local TemperatureLimit = alimit	-- temperature triggering freshness saving

	-- methods
	local function StopSurvey()
		SelLog.log(self.getName() .. " : Fin de la surveillance de la température")
		timer.TaskRemove( LaunchSurvey )
		timer.TaskRemove( StopSurvey )
		probe.TaskRemove( surveyTemperature )
	end

	local function surveyTemperature()
		if probe.get() > TemperatureLimit then
			SelLog.log( self.getName() .. " : " .. probe.get() .. ", les volets se ferment")
			self.My()
			StopSurvey()

			timer.TaskAdd( HMonitoringStop, self.Up )
			SelLog.log( self.getName() .. " : Les volets s'ouvriront à " .. HMonitoringStop )
		elseif DEBUG then
			SelLog.log( self.getName() .. " : " .. probe.get() .. ' -> trop basse' )
		end
	end

	local function LaunchSurvey()
		SelLog.log(self.getName() .." : Début de la surveillance de la température")
		timer.TaskRemove( LaunchSurvey )
		probe.TaskAdd( surveyTemperature )
	end

	function self.LaunchSurveyAt()
		local cur = atimer.Current()

		if cur < HMonitoringStart then		-- Before monitoring period
			SelLog.log( self.getName() .. " : La surveillance commencera à " .. HMonitoringStart )
			timer.TaskAdd( HMonitoringStart, LaunchSurvey )

			SelLog.log( "Et se terminera à " .. HMonitoringStop )
			timer.TaskAdd( HMonitoringStop, StopSurvey )

			probe.TaskRemove( surveyTemperature )
		elseif cur < HMonitoringStop then	-- during
			LaunchSurvey()

			SelLog.log( self.getName() .. " : La surveillance se terminera à " .. HMonitoringStop )
			timer.TaskAdd( HMonitoringStop, StopSurvey )
		else								-- after
			StopSurvey()
		end
	end

	function self.getTimer()
		return timer
	end

	return self
end

