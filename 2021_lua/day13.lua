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

local term   = require("term")
local utils  = require("utils")

local _M = {}

local function parse_file(file)
	local board,fold = {}, {}
	local state = "dots"
	local mimax = {minx=1000, miny=1000, maxy=0, maxx=0}
	for line in file:lines() do
		if state == "dots" then
			if line == "" then
				state = "fold"
			else
				local x,y = line:match("(%d+),(%d+)")
				x,y = tonumber(x), tonumber(y)
				if x < mimax.minx then mimax.minx = x end
				if y < mimax.miny then mimax.miny = y end
				if x > mimax.maxx then mimax.maxx = x end
				if y > mimax.maxy then mimax.maxy = y end
				board[line] = true
			end
		elseif state == "fold" then
			local dir,val = line:match("fold along (.)=(%d+)")
			val = tonumber(val)
			table.insert(fold, {dir=dir, val=val})
	end
	end
	return {board=board, fold=fold, mimax=mimax}
end

local function print_board(b, pre)
	if graphic then
		term.clear()
		term.cursor['goto'](1, 1)
		if pre then print(pre) end
	end
	local mimax,board = b.mimax, b.board
	for y=mimax.miny,mimax.maxy do
		for x=mimax.minx,mimax.maxx do
			if board[string.format("%d,%d", x, y)] then
				io.write("#")
			else
				io.write(" ")
			end
		end
		io.write("|\n")
	end
end

local function fold(b, pos, foo1)
	local b2 = utils.table.copy_deep(b)
	for coord,_ in pairs(b.board) do
		local x,y = coord:match("(%d+),(%d+)")
		x,y = tonumber(x),tonumber(y)
		foo1(b2, x,y)
	end
	return b2
end

local function foldy(b,pos)
	b.mimax.maxy = pos-1
	return fold(b, pos, function(b,x,y)
	if y > pos then
		b.board[string.format("%d,%d", x, y)] = nil
		b.board[string.format("%d,%d", x, pos-(y-pos))] = true
		if pos-(y-pos) < b.mimax.miny then b.mimax.miny = pos-(y-pos) end
	end
end)
end

local function foldx(b,pos)
	b.mimax.maxx = pos-1
	return fold(b, pos, function(b,x,y)
	if x > pos then
		b.board[string.format("%d,%d", x, y)] = nil
		b.board[string.format("%d,%d", pos-(x-pos), y)] = true
		if pos-(x-pos) < b.mimax.minx then b.mimax.minx = pos-(x-pos) end
	end
end)
end

local function board_count(board)
	return utils.set.len(board)
end

function _M.part1(file)
	-- file = io.open("./day13.dat.testing")
	local b = parse_file(file)

	if b.fold[1].dir == "x" then
		b = foldx(b, b.fold[1].val)
	else
		b = foldy(b, b.fold[1].val)
	end
	return board_count(b.board)
end

function _M.part2(file)
	-- file = io.open("./day13.dat.testing")
	local b = parse_file(file)
	if graphic then print_board(b) end

	local fold_ = utils.table.copy_deep(b.fold)
	for _,f in ipairs(fold_) do
		if f.dir == "x" then
			b = foldx(b, f.val)
		else
			b = foldy(b, f.val)
		end
		if graphic then
			print_board(b, "Part2")
			os.execute("sleep 0.1")
		end
	end
	print()
	print_board(b)
	print()
end

return _M
