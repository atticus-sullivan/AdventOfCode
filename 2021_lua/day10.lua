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
	local function searchList(list, char)
		for i,v in ipairs(list) do
			if v == char then return i end
		end
		return nil
	end
	local closing = {")", "]", "}", ">"}
	local opening = {"(", "[", "{", "<"}
	local score   = {[")"]=3, ["]"]=57, ["}"]=1197, [">"]=25137}
	local stack
	local sum = 0
	for line in file:lines() do
		stack = {}
		for x in line:gmatch(".") do
			local  c = searchList(closing,x)
			if     c and stack[#stack] == opening[c] then table.remove(stack)
			elseif c and stack[#stack] ~= opening[c] then sum = sum + score[x] break
			elseif searchList(opening,x)                  then table.insert(stack, x)
			end
		end
	end
	return sum
end

function _M.part2(file)
	local function searchList(list, char)
		for i,v in ipairs(list) do
			if v == char then return i end
		end
		return nil
	end
	local closing = {")", "]", "}", ">"}
	local opening = {"(", "[", "{", "<"}
	local score   = {["("]=1, ["["]=2, ["{"]=3, ["<"]=4}
	local stack,failed
	local sums = {}
	for line in file:lines() do
		stack,failed = {},false
		for x in line:gmatch(".") do
			local  c = searchList(closing,x)
			if     c and stack[#stack] == opening[c] then table.remove(stack)
			elseif c and stack[#stack] ~= opening[c] then failed = true break
			elseif searchList(opening,x)        then table.insert(stack, x)
			end
		end
		if not failed then
			local sum = 0
			for i=#stack,1,-1 do
				sum = 5*sum + score[stack[i]]
			end
			table.insert(sums,sum)
		end
	end
	table.sort(sums)
	return sums[#sums//2+1]
end

return _M
