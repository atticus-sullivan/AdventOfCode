-- Advent Of Code 2021
-- Copyright (C) 2024  Lukas Heindl
-- 
-- This program is free software: you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation, either version 3 of the License, or
-- (at your option) any later version.
-- 
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
-- 
-- You should have received a copy of the GNU General Public License
-- along with this program.  If not, see <http://www.gnu.org/licenses/>.

assert(arg[1] and arg[1]:match("^%d%d$"), "Day must be given as first parameter and has to be a two digit number")
assert(not arg[2] or arg[2] == "1" or arg[2] == "2" or arg[2] == "0", "Part to execute must be 0(both), 1 or 2")

local day = require("day"..arg[1])

graphic = arg[3] == "g"

local file, r1,r2
if not arg[2] or arg[2] == "1" or arg[2] == "0" then
	file = io.open("problems/day"..arg[1]..".dat")
	local s = os.clock()
	r1,r2 = day.part1(file)
	local e = os.clock()
	print("Part1:", r1, e-s)
	file:close()
end

if not arg[2] or arg[2] == "2" or arg[2] == "0" then
	file = io.open("problems/day"..arg[1]..".dat")
	local s = os.clock()
	if not r2 then
		r2 = day.part2(file)
	end
	local e = os.clock()
	print("Part2:", r2, e-s)
	file:close()
end
