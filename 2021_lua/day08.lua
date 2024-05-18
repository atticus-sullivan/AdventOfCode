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
	local cnt = 0
	for line in file:lines() do
		local state = 1
		for signal in line:gmatch("[abcdefg|]+") do
			if signal == "|" then
				state = state+1
			elseif state == 2 then
				if #signal == 2  or #signal == 4 or #signal == 3 or #signal == 7 then
					cnt = cnt+1
				end
			elseif state ~= 1 then
				error(string.format("Error while parsing line %s signal %s state %d", line, signal, state))
			end
		end
	end
	return cnt
end

function _M.part2(file)
	local keys = {"a", "b", "c", "d", "e", "f", "g"}
	-- returns the amount which is contained in a but not in b
	local function get_diffs(a,b)
		local diffs = 0
		for _,k in ipairs(keys) do
			if a[k] ~= b[k] and a[k] == true then diffs = diffs + 1 end
		end
		return diffs
	end
	local function string_to_table(a)
		local r = {}
		for _,k in ipairs(keys) do
			r[k] = a:match(k) ~= nil and true or false
		end
		return r
	end
	local function string_to_array(s)
		local ret = {}
		for x in s:gmatch(".") do
			table.insert(ret, x)
		end
		return ret
	end
	local function table_to_array(t)
		local ret = {}
		for k,v in pairs(t) do
			if v then table.insert(ret, k) end
		end
		return ret
	end
	local function array_to_sorted_string(t)
		table.sort(t)
		return table.concat(t,"")
	end
	local function resolve_with(unknown,known,amount)
		for i,s in ipairs(unknown) do
			if get_diffs(known, s) == amount then
				return i
			end
		end
		return nil
	end

	local sum = 0
	local map,mapR,fives,sixes
	local display,state
	for line in file:lines() do
		display,state = {},1
		map,mapR,fives,sixes = {},{},{},{}
		for signal in line:gmatch("[abcdefg|]+") do
			if signal == "|" then state = state+1
			elseif state == 1 then
				if     #signal == 2 then mapR[1] = string_to_table(signal)
				elseif #signal == 3 then mapR[7] = string_to_table(signal)
				elseif #signal == 4 then mapR[4] = string_to_table(signal)
				elseif #signal == 7 then mapR[8] = string_to_table(signal)
				elseif #signal == 5 then table.insert(fives, string_to_table(signal))
				elseif #signal == 6 then table.insert(sixes, string_to_table(signal))
				end
			elseif state == 2 then
				table.insert(display, signal)
			else
				error(string.format("Error while parsing line %s signal %s state %d", line, signal, state))
			end
		end

		local j
		j = resolve_with(sixes, mapR[1], 1)
		assert(j, "6 hasn't been found")
		mapR[6] = sixes[j]
		table.remove(sixes, j)

		j = resolve_with(fives, mapR[6], 1)
		assert(j, "5 hasn't been found")
		mapR[5] = fives[j]
		table.remove(fives, j)

		j = resolve_with(fives, mapR[7], 1)
		assert(j, "3 hasn't been found")
		mapR[2] = fives[j]
		table.remove(fives, j)

		j = resolve_with(sixes, mapR[4], 1)
		assert(j, "0 hasn't been found")
		mapR[0] = sixes[j]
		table.remove(sixes, j)

		mapR[3] = fives[1]
		mapR[9] = sixes[1]

		for k,v in pairs(mapR) do
			map[array_to_sorted_string(table_to_array(v))] = k
		end

		local num = ""
		for _,out in ipairs(display) do
			num = num .. tostring(map[array_to_sorted_string(string_to_array(out))])
		end
		sum = sum + tonumber(num)
	end
	return sum
end

return _M
