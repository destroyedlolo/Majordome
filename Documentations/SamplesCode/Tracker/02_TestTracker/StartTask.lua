-- This task will be called when the tracker start to check
-- (status changed to CHECKING)
--
-- The tracker to follow
-->> whenStarted=TestTracker
--
-- Disable this script
--->> disabled

print("The tracker is started, while tracker status was", MAJORDOME_TRACKER_STATUS)
