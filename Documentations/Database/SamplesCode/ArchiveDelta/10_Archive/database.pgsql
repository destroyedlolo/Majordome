-- PostgreSQL database access definition
--
-- name of the database declaration
-- if not set, takes the filename
--->> name=toto
--
-- Connection string : see PostgreSQL documentation
-- Something like :
--	postgresql://[user[:password]@][[host][:port][,...]][/dbname][?name=value[&...]]
-- to test it, you can try something like:
--	psql -t postgresql://test@torchwood.chez.moi -c 'select current_date'
-->> pgConnectionstring=postgresql://domestik2:domestik2@testarch.local/www
