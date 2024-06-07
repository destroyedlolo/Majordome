**Tracker** example.

In this exemple, `TestTracker` will be started every `15s` and stopped every `25s`.<br>
It is listening for `Test` topic and will goes **Done** if received value is higher than 10.

2 rendez-vous are also created rendez-vous to notify when the tracker is disabled or enabled.

--------

In addition, `Control` topic quand be used to query or change tracker's status.
  * **STATUS** : displays the tracker status
  * **MANUAL** : disables timers so the tracker stays in the same state
  * **AUTO** : activates timers so the tracker switches automatically b/w waiting and checking state
  * **WAITING** or **STOP** : the tracker waits for a start signal
  * **CHECKING** or **START** : the tracker checks for incoming values
  * **DONE** : expected value found
