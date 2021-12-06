assert(arg[1] and arg[1]:match("^%d%d$"), "Day must be given as first parameter and has to be a two digit number")
assert(not arg[2] or arg[2] == "1" or arg[2] == "2", "Part to execute must be none(both), 1 or 2")

-- global helper functions
function ShallowCopyTable(t)
	local res = {}
	for i,v in pairs(t) do
		res[i] = v
	end
	return res
end
function DeepCopyTable(t)
	if type(t) ~= "table" then return t end
	local res = {}
	for i,v in pairs(t) do
		res[i] = DeepCopyTable(v)
	end
	return res
end
function PrintTable(t, shift)
	local function printTable(t, indent)
		if type(t) ~= "table" then io.write(string.rep(" ", indent), t, "\n") return end
		local res = {}
		for k,v in pairs(t) do
			io.write(string.rep(" ", indent), k, " -> \n")
			printTable(v, indent+(shift or 2))
		end
	end
	printTable(t, 0)
end

local day = require("day"..arg[1])

local file
if not arg[2] or arg[2] == "1" then
	file = io.open("day"..arg[1]..".dat")
	print("Part1:", day.part1(file))
	file:close()
end

if not arg[2] or arg[2] == "2" then
	file = io.open("day"..arg[1]..".dat")
	print("Part2:", day.part2(file))
	file:close()
end
