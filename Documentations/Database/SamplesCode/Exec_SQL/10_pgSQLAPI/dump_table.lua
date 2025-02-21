-- Launch this task at startup
-->> RunAtStartup
--
-->> need_pgSQL=database

local ret = database:doSQL("select * from ntest;")

print ""
print "table ntest content"
print "-------------------"
print ""

for _,row in ipairs(ret) do
	local s;
	for _,field in ipairs(row) do
		if not s then
			s = field
		else
			s = s .. '\t' .. field
		end
	end
	print(s)
end
