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

function _M.part1(file)
	local ret = 0
	local n1,n2 = file:read("n"),file:read("n")
	while n2 do
		if n1 < n2 then ret = ret+1 end
		n1 = n2
		n2 = file:read("n")
	end
	return ret
end

function _M.part2(file)
	local ret = 0
	local lastSum = nil
	local sums = {0, 0, 0}
	local n = file:read("n")
	sums[1] = n
	n = file:read("n")
	sums[2] = n
	sums[1] = sums[1] + n
	n = file:read("n")
	while n do
		sums[1] = sums[1] + n
		sums[2] = sums[2] + n
		sums[3] = sums[3] + n

		-- sum[1] is finished
		if lastSum and lastSum < sums[1] then ret = ret+1 end
		lastSum = sums[1]
		table.remove(sums, 1)
		sums[3] = 0
		n = file:read("n")
	end
	return ret
end

return _M
