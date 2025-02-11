-- Change the timer's time in order to test its handlers are calling
--
-->> need_timer=at2350
--
-- This task will run at Majordome's startup
-->> RunAtStartup

local time = os.date('*t')
print("It's currently :", ("%02d:%02d"):format(time.hour, time.min))

-- Changing timer's
-- As we are setting to the current time, the timer is overdue

print("Changing timer's target")
at2350:setAtHM(time.hour, time.min)
