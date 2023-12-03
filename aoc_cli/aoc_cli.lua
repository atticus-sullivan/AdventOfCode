local p = arg[0]:match("^(.*/).-$") or ""
package.path = ('%s?.lua;%s'):format(p,package.path)
package.cpath = ('%s?.so;%s'):format(p,package.cpath)
local utils = require 'utils'

local https = require 'ssl.https'
local ltn12 = require "ltn12"
local lxp   = require "lxp.lom"
local cli   = require 'cliargs'


local function get_input(key, year, day, path)
	local url,session,user_agent,file,fn,code

	url = "https://adventofcode.com/"..year.."/day/"..day:gsub("^0+", "").."/input"
	user_agent = "advent-of-code-data"

	session = utils.getSession(key, path)

	fn = "day"..day..".dat"
	file = io.open(fn, "w")
	_,code,_,_ = https.request{url = url, headers={cookie="session="..session, ["user-agent"]=user_agent}, sink=ltn12.sink.file(file)}
	-- sink.file already closes the file in the end

	print("Status code:", code)
end

local function get_problem(key, year, day, printing, path)

	local M = require("get_problem")
	if not M then
		print("You don't have the get problem module. This is excluded from the published version to avoid stealing the Website from Eric")
		return
	end
	M.get_problem(key, year, day, printing, path)
end


local function send_sol(key, year, day, sol, level, path)
	local url,session,user_agent,code

	url = "https://adventofcode.com/"..year.."/day/"..day:gsub("^0+", "").."/answer"
	user_agent = "advent-of-code-data"

	session = utils.getSession(key, path)

	local output = {}
	local content = string.format('level=%s&answer=%s', level, sol)
	local headers = {
		cookie="session="..session,
		["Content-type"]="application/x-www-form-urlencoded",
		["Content-Length"]=#content,
		["user-agent"]=user_agent,
	}
	_,code,_,_ = https.request{url=url, method="POST", headers=headers, sink=ltn12.sink.table(output), source=ltn12.source.string(content)}

	if table.concat(output, ""):match("That's the right answer!") then print("SUCCESS") else print("WRONG") end

	print("Status code:", code)

	-- print the text inside the article section
	local root = lxp.parse(table.concat(output, ""):gsub("&", "").."")
	local res = {}
	utils.find(root, function (n) return n["tag"] == "article" end, res, {})
	for k,_ in pairs(res) do utils.printStrings(k) end
end

local function isNumber(key, value)
	if not tonumber(value) then print("Option " .. key .. " must be a number (was "..value..")") return nil, "" end
	return value
end

cli:set_name("aoc_cli.lua")
cli:splat("date", "day and year to read from (default: today)", nil, 2)
cli:option("-s, --solution=SOLUTION", "send a solution to the server")
cli:option("-p, --part=PART", "set the part of the exercise", "1")
cli:flag("-f, --fetch", "fetch the problem statement")
cli:flag("-i, --input", "get the input file")
cli:flag("--print-problem", "print the problem statement after fetching")
cli:option("-k, --key=KEY", "the line in the .sess file to use", "1", isNumber)

local args,err = cli:parse(arg)
if err then
	print(err)
	os.exit(0)
end
args["day"]  = args["date"][1] or os.date("%d")
args["year"] = args["date"][2] or os.date("%Y")

-- check parameters for validity
assert(
	args["day"] and args["year"] and tonumber(args["day"]) and tonumber(args["day"]) <= 31 and tonumber(args["day"]) > 0,
	"First parameter is the day, second is the year. Both are optional"
)
assert(args["p"] == "1" or args["p"] == "2", "Part has to be 1 or 2!")

-- correct parameter type
args["key"] = tonumber(args["key"])

-- to get the .sess file from the location where the script is located
local path = arg[0]:match("(.*/).-") or ""

local ranFoo = false
if args["solution"] then
	if ranFoo then print() else ranFoo = true end
	print(string.format("Submitting solution %s for part %s on day %s year %s (key:%d)", args["solution"], args["part"], args["day"], args["year"], args["key"]))
	send_sol(args["key"], args["year"], args["day"], args["solution"], args["part"], path)
end
if args["fetch"] then
	if ranFoo then print() else ranFoo = true end
	print(string.format("Fetch problem statement for day %s year %s (key:%d)", args["day"], args["year"], args["key"]))
	get_problem(args["key"], args["year"], args["day"], args["print-problem"], path)
end
if args["input"] then
	if ranFoo then print() else ranFoo = true end
	print(string.format("Fetch input for day %s year %s (key:%d)", args["day"], args["year"], args["key"]))
	get_input(args["key"], args["year"], args["day"], path)
end

if not ranFoo then
	print("Specify one of the actions seen in help page to execute some things")
end
