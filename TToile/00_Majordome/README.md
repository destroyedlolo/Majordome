This package contains **Majordome**'s own management tasks.<br>

- Log to files with everyday rotation. Files are `<clientid>.log_<today date>` in **/tmp**.
- cleaning of logs older than 30 days

> [!TIP]
> This containers can be used out of the box in every Majordome projects.

> [!NOTE]
> [PurgeLog](PurgeLog.lua) needs **LuaFileSystem**
