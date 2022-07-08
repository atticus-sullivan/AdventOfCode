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

local function print_board(b, path)
	path = path or {}
	for y = b.mima.miny, b.mima.maxy do
		for x = b.mima.minx, b.mima.maxx do
			local c = string.format("%d,%d", x,y)
			if path[c] then
				io.write(b.board[c].risk)
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

local function pq_empty(pq)
	return not pq.q[pq.min] or #pq.q[pq.min] <= 0
end

local function pq_insert(pq, ele, key)
	-- print("set", ele.x, ele.y, "to", key)
	ele.acc = key
	if not pq.q[key] then
		pq.q[key] = {}
	end
	table.insert(pq.q[key], ele)
	if key < pq.min then pq.min = key end
	-- print(pq.min, pq.q[pq.min], pq_empty(pq))
end

local function pq_decKey(pq, ele, key)
	local ind = 0
	for i,v in ipairs(pq.q[ele.acc]) do
		if v == ele then
			ind = i
			break
		end
	end
	table.remove(pq.q[ele.acc], ind)
	pq_insert(pq, ele, key)
end

local function pq_delMin(pq)
	local r = table.remove(pq.q[pq.min])
	if #pq.q[pq.min] <= 0 then
		pq.q[pq.min] = nil

		pq.min = 100000000000000000000
		for k,_ in pairs(pq.q) do
			if k < pq.min then pq.min = k end
		end
	end
	return r
end

local function dijkstra(b)
	local pq = {q={}, min=1000000000}
	pq_insert(pq, b.board["0,0"], 0)

	while not pq_empty(pq) do
		local v = pq_delMin(pq)
		-- print("v", v.x, v.y)

		for _,i in ipairs({{x=1,y=0},{x=-1,y=0},{x=0,y=1},{x=0,y=-1}}) do
			if on_board(b, {x = v.x+i.x,  y = v.y+i.y}) then
				local c = string.format("%d,%d", v.x+i.x, v.y+i.y)
				local w = b.board[c]
				local acc = v.acc + w.risk
				-- print("w", w.x, w.y, w.acc, w.risk, type(w.acc))
				if not w.acc then
					pq_insert(pq, w, acc)
				elseif acc < w.acc then
					pq_decKey(pq, w, acc)
				end
			end
		end
	end
	return b.board[string.format("%d,%d", b.mima.maxx, b.mima.maxy)].acc
end

function _M.part1(file)
	-- file = io.open("./day15.dat.testing")
	local b = parse_file(file)
	-- print_board(b)
	return dijkstra(b)
end

function _M.part2(file)
	-- file = io.open("./day15.dat.testing")
	local b = parse_file(file)
	inc(b)
	-- print_board(b)
	return dijkstra(b)
end

return _M
