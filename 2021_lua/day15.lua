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
local utils  = require("aocutils")

local _M = {}

local function parse_file(file)
	local mima = {maxx=0, maxy=0, minx=0, miny=0}
	local board = {}

	local y = 0
	for line in file:lines() do
		local x = 0
		for c in line:gmatch(".") do
			local co = string.format("%d,%d", x,y)
			board[co] = {risk=tonumber(c), acc=nil, x=x, y=y}
			x = x+1
		end
		mima.maxx = x-1
		y = y+1
	end
	mima.maxy = y-1

	return {mima=mima, board=board}
end

local function inc(b)
	for y = b.mima.miny, b.mima.maxy do
		for x = b.mima.minx, b.mima.maxx do
			local c1 = string.format("%d,%d", x,y)
			for i=1,4 do
				local c2 = string.format("%d,%d", x+i*(b.mima.maxx+1),y)
				b.board[c2] = {risk=(b.board[c1].risk+i-1)%9+1, acc=nil, x=x+i*(b.mima.maxx+1), y=y}
			end
		end
	end
	b.mima.maxx = b.mima.maxx + (b.mima.maxx+1)*4

	for y = b.mima.miny, b.mima.maxy do
		for x = b.mima.minx, b.mima.maxx do
			local c1 = string.format("%d,%d", x,y)
			for i=1,4 do
				local c2 = string.format("%d,%d", x, y+i*(b.mima.maxy+1))
				b.board[c2] = {risk=(b.board[c1].risk+i-1)%9+1, acc=nil, x=x, y=y+i*(b.mima.maxy+1)}
			end
		end
	end
	b.mima.maxy = b.mima.maxy + (b.mima.maxy+1)*4
end

local function print_board(b, pq, pre)
	local function in_pq(pq, c)
		if not c.acc then return false end
		if not pq.q[c.acc] then return false end
		for _,e in ipairs(pq.q[c.acc]) do
			if e == c then return true end
		end
		return false
	end
	if graphic then
		term.clear()
		term.cursor['goto'](1, 1)
		if pre then print(pre) end
	end
	pq = pq or {}
	for y = b.mima.miny, b.mima.maxy do
		for x = b.mima.minx, b.mima.maxx do
			local c = string.format("%d,%d", x,y)
			if in_pq(pq, b.board[c]) and graphic then
				io.write(colors.red(b.board[c].risk))
			else
				io.write(b.board[c].risk)
			end
		end
		io.write("\n")
	end
end

local function on_board(b, c)
	return b.mima.minx <= c.x and c.x <= b.mima.maxx and
		b.mima.miny <= c.y and c.y <= b.mima.maxy
end

local function dijkstra(b, pre)
	local pq = {q={}, min=1000000000}
	utils.pq.insert(pq, b.board["0,0"], 0)

	if graphic and pre then
		print_board(b, pq, pre)
		os.execute("sleep 0.1")
	end

	while not utils.pq.empty(pq) do
		local v = utils.pq.delmin(pq)
		-- print("v", v.x, v.y)

		for _,i in ipairs({{x=1,y=0},{x=-1,y=0},{x=0,y=1},{x=0,y=-1}}) do
			if on_board(b, {x = v.x+i.x,  y = v.y+i.y}) then
				local c = string.format("%d,%d", v.x+i.x, v.y+i.y)
				local w = b.board[c]
				local acc = v.acc + w.risk
				-- print("w", w.x, w.y, w.acc, w.risk, type(w.acc))
				if not w.acc then
					utils.pq.insert(pq, w, acc)
				elseif acc < w.acc then
					utils.pq.decKey(pq, w, acc)
				end
				if graphic and pre then
					print_board(b, pq, pre)
					os.execute("sleep 0.01")
				end
			end
		end
	end
	return b.board[string.format("%d,%d", b.mima.maxx, b.mima.maxy)].acc
end

function _M.part1(file)
	if graphic then
		file = io.open("./day15.dat.testing")
	end
	local b = parse_file(file)
	-- print_board(b)
	return dijkstra(b, "Part1")
end

function _M.part2(file)
	-- file = io.open("./day15.dat.testing")
	local b = parse_file(file)
	inc(b)
	-- print_board(b)
	return dijkstra(b, false)
end

return _M
