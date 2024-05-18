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
	local vert = {}
	for line in file:lines() do
		local i = 1
		for v in line:gmatch(".") do
			if vert[i] == nil then
				vert[i] = {}
				vert[i]["0"] = 0
				vert[i]["1"] = 0
			end
			vert[i][v] = vert[i][v] + 1
			i = i + 1
		end
	end

	local gamma,epsilon = {}, {}
	for _,v in ipairs(vert) do
		if v["0"] > v["1"] then
			table.insert(gamma, "0")
			table.insert(epsilon, "1")
		else
			table.insert(epsilon, "0")
			table.insert(gamma, "1")
		end
	end
	return tonumber(table.concat(gamma), 2) * tonumber(table.concat(epsilon), 2)
end

local function copyTable(t)
	local res = {}
	for _,v in ipairs(t) do
		table.insert(res, v)
	end
	return res
end

function _M.part2(file)
	local function mostCommon(lines, index)
		local ones  = 0
		local zeros = 0
		for _,line in pairs(lines) do
			if line:sub(index, index) == "1" then ones = ones+1 else zeros = zeros+1 end
		end
		return ones >= zeros and "1" or "0"
	end
	local lines = {}
	for l in file:lines() do table.insert(lines, l) end
	local lines2 = copyTable(lines)

	for index = 1,#lines[1] do
		local remove = {}
		local common = mostCommon(lines, index)
		for i,v in ipairs(lines) do
			if v:sub(index,index) ~= common then table.insert(remove, 1, i) end
		end
		for _,v in ipairs(remove) do table.remove(lines, v) end
		if #lines == 1 then break end
	end
	local oxygen = tonumber(lines[1], 2)

	for index = 1,#lines2[1] do
		local remove = {}
		local common = mostCommon(lines2, index)
		for i,v in ipairs(lines2) do
			if v:sub(index,index) == common then table.insert(remove, 1, i) end
		end
		for _,v in ipairs(remove) do table.remove(lines2, v) end
		if #lines2 == 1 then break end
	end
	local co2 = tonumber(lines2[1], 2)
	return co2*oxygen
end

return _M
