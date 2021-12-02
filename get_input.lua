local https = require 'ssl.https'
local ltn12 = require("ltn12")

print(arg[0])

local url,session,user_agent,file,fn,code,day,year,path

path = arg[0]:match("(.*/).-") or ""

day  = arg[1] or os.date("%d")
year = arg[2] or os.date("%Y")

assert(tonumber(day) and tonumber(day) <= 31 and tonumber(day) > 0, "First parameter is the day, second is the year. Both are optional")

url = "https://adventofcode.com/"..year.."/day/"..day:gsub("^0+", "").."/input"
user_agent = "advent-of-code-data"

file = io.open(path..".sess")
session = file:read("l")
file:close()

fn = "day"..day..".dat"
file = io.open(fn, "w")
_,code,_,_ = https.request{url = url, headers={cookie="session="..session, ["user-agent"]=user_agent}, sink=ltn12.sink.file(file)}
-- sink.file already closes the file in the end

for line in io.lines(fn) do
	print(line)
end
print("Status code:", code)
