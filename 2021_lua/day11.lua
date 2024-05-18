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

local colors = require("term.colors")
local term   = require("term")

local _M = {}

local function parse_file(file)
	local field = {}
	for line in file:lines() do
		local row = {}
		for x in line:gmatch(".") do
			table.insert(row, tonumber(x))
		end
		table.insert(field, row)
	end
	return field
end

local function print_field(field, pre)
	if graphic then
		term.clear()
		term.cursor['goto'](1, 1)
		if pre then print(pre) end
	end
	for _,r in ipairs(field) do
		for _,c in ipairs(r) do
			if c == 0 then
				io.write(colors.red(string.format("%1d", c)))
			else
				io.write(string.format("%1d", c))
			end
		end
		io.write("\n")
	end
end

local function step(field)
	local amount = 0
	local function inc(y,x)
		if not field[y] or not field[y][x] then return end

		field[y][x] = field[y][x] + 1

		if field[y][x] == 10 then
			amount = amount + 1
			inc(y+0,x+1)
			inc(y+0,x-1)
			inc(y+1,x+0)
			inc(y+1,x+1)
			inc(y+1,x-1)
			inc(y-1,x+0)
			inc(y-1,x+1)
			inc(y-1,x-1)
		end
	end
	for y,_ in ipairs(field) do
		for x,_ in ipairs(field[y]) do
			inc(y,x)
		end
	end
	for y,_ in ipairs(field) do
		for x,_ in ipairs(field[y]) do
			if field[y][x] > 9 then field[y][x] = 0 end
		end
	end
	return amount
end

function _M.part1(file)
	local field = parse_file(file)
	if graphic then print_field(field, "Part1:") end

	local sum = 0
	for i=1,100 do
		sum = sum + step(field)
		if graphic then
			print_field(field, "Part2:")
			os.execute("sleep 0.05")
		end
	end
	return sum
end

function _M.part2(file)
	local function all_flash(field)
		for _,r in ipairs(field) do
			for _,c in ipairs(r) do
				if c ~= 0 then return false end
			end
		end
		return true
	end
	local field = parse_file(file)
	if graphic then print_field(field, "Part2:") end

	local i = 1
	while true do
		step(field)
		if graphic then
			print_field(field, "Part2:")
			os.execute("sleep 0.01")
		end
		if all_flash(field) then return i end
		i = i + 1
	end
	return nil
end

return _M
