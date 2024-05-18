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

local function inc(x, k, i)
	i = i or 1 -- default to 1 of argument isn't given
	if type(x[k]) == "number" then
		x[k] = x[k] + i
	else
		x[k] = i
	end
end

local function parse_file(file)
	local prod,alph = {}, {}

	local temp = {}
	local lastc = file:read(1)
	inc(alph, lastc)
	local c = file:read(1)
	while c ~= "\n" do
		inc(alph, c)
		inc(temp, lastc..c)
		lastc = c
		c = file:read(1)
	end

	file:read("l")

	for line in file:lines() do
		local f1,f2, t = line:match("(.)(.) %-> (.)")
		prod[f1..f2] = t
	end
	return temp, prod, alph
end

local function find_prod(prod, k)
	return prod[k]
end

-- only store the combinations and how often they occur
-- also store a map alphabet -> occurences of char
local function foo(steps, temp, prod, alph)
	for _=1,steps do
		local temp2 = {}
		for k,v in pairs(temp) do
			local f1,f2 = k:sub(1,1), k:sub(2,2)
			local t = find_prod(prod, k)
			inc(temp2, f1..t, v)
			inc(temp2, t..f2, v)
			inc(alph, t, v)
		end
		temp = temp2
	end

	local min,max = 1000000000000000000000,0
	for _,v in pairs(alph) do
		if type(v) == "number" then
			if v < min then min = v end
			if v > max then max = v end
		end
	end
	return max-min
end

function _M.part1(file)
	-- file = io.open("./day14.dat.testing")
	local temp,prod,alph = parse_file(file)
	return foo(10, temp, prod, alph)
end

function _M.part2(file)
	-- file = io.open("./day14.dat.testing")
	local temp,prod,alph = parse_file(file)
	return foo(40, temp, prod, alph)
end

return _M
