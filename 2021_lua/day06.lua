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

local _M = {}

local function parseFile(file)
	local arr = {0,0,0,0,0,0,0,0,0,0}
	for x in file:read("l"):gmatch("(%d+)") do
		arr[x+1] = arr[x+1] + 1
	end
	return arr
end

local function part(file, days)
	--                  0,1,2,3,4,5,6,7,8,9
	local population = parseFile(file)
	-- io.write(string.format("%-3d", 0))
	-- printTable(population)
	for _=1,days do
		local t = table.remove(population, 1)
		population[9] = t
		population[7] = population[7] + t
		-- io.write(string.format("%-3d", i))
		-- printTable(population)
	end
	local sum = 0
	for _,v in pairs(population) do
		sum = sum + v
	end
	return sum
end

function _M.part1(file) return part(file, 80) end
function _M.part2(file) return part(file, 256) end

return _M
