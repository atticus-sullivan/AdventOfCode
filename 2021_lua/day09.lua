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

local colors = require("term.colors")
local utils  = require("utils")

local function parse_file(file)
	local ret = {}
	for line in file:lines() do
		local row = {}
		for x in line:gmatch("%d") do
			table.insert(row, tonumber(x))
		end
		table.insert(ret, row)
	end
	return ret
end

function _M.part1(file, pr)
	-- pr = true
	file = io.open("day09.dat")
	local field = parse_file(file)
	local sum = 0
	for i,v in ipairs(field) do
		for j,w in ipairs(v) do
			if (not field[i]   or not field[i][j-1] or field[i][j-1] > w) and
			   (not field[i]   or not field[i][j+1] or field[i][j+1] > w) and
			   (not field[i-1] or not field[i-1][j] or field[i-1][j] > w) and
			   (not field[i+1] or not field[i+1][j] or field[i+1][j] > w) then
				if pr then io.write(colors.red(w)) end
				sum = sum + w+1
			elseif pr then io.write(w)
			end
		end
		if pr then io.write("\n") end
	end
	return sum
end

local function print_marked_field(field, set)
	for y,v in ipairs(field) do
		for x,w in ipairs(v) do
			local c = string.format("%d|%d", y, x)
			if set[c] then
				io.write(colors.red(w))
			else
				io.write(w)
			end
		end
		io.write("\n")
	end
end

function _M.part2(file, pr)
	local function mark_adjacent(field, set, cur, prev)
		if not field[cur.y] or not field[cur.y][cur.x] then return end

		for _,yc in ipairs({-1,1}) do
			local c = string.format("%d|%d", cur.y+yc, cur.x)
			if field[cur.y+yc] and cur.y + yc ~= prev.y and field[cur.y+yc][cur.x] < 9 and not set[c] and field[cur.y][cur.x] < field[cur.y+yc][cur.x] then
				set[c] = true
				mark_adjacent(field, set, {y=cur.y+yc, x=cur.x}, cur)
			end
		end
		for _,xc in ipairs{-1,0,1} do
			local c = string.format("%d|%d", cur.y, cur.x+xc)
			if field[cur.y][cur.x+xc] and cur.x + xc ~= prev.x and field[cur.y][cur.x+xc] < 9 and not set[c] and field[cur.y][cur.x] < field[cur.y][cur.x+xc] then
				set[c] = true
				mark_adjacent(field, set, {y=cur.y, x=cur.x+xc}, cur)
			end
		end
	end

	file = io.open("day09.dat")

	local field = parse_file(file)
	local sizes = {}
	for i,v in ipairs(field) do
		for j,w in ipairs(v) do
			if (not field[i]   or not field[i][j-1] or field[i][j-1] > w) and
			   (not field[i]   or not field[i][j+1] or field[i][j+1] > w) and
			   (not field[i-1] or not field[i-1][j] or field[i-1][j] > w) and
			   (not field[i+1] or not field[i+1][j] or field[i+1][j] > w) then
			   local set = {[string.format("%d|%d", i,j)]=true}
			   mark_adjacent(field, set, {x=j,y=i}, {x=-10,y=-10})
			   -- print_marked_field(field, set) print()
			   table.insert(sizes, utils.set.len(set))
			   -- os.exit()
			end
		end
	end
	table.sort(sizes, function(a,b) return a > b end)
	return sizes[1] * sizes[2] * sizes[3]
end

return _M
