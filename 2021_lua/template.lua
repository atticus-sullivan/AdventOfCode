local utils = require"aocutils"

local _M = {}

local function parse_file(file)
	for line in file:lines() do
		print(line)
	end
end

function _M.part1(file)
	parse_file(file)
	print(file)
end

function _M.part2(file)
	parse_file(file)
	print(file)
end

return _M
