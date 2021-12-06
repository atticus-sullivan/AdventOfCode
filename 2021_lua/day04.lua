local _M = {}

local function parseFile(file)
	local numbers = {}
	for x in file:read("l"):gmatch("%d+") do table.insert(numbers, x) end
	local boards = {}
	local running = true
	while running do
		if not file:read("l") then break end
		local board = {}
		for _ = 1,5 do
			local row = {}
			for x in file:read("l"):gmatch("%d+") do table.insert(row, {number=x, marked=false}) end
			table.insert(board, row)
		end
		table.insert(boards, board)
	end
	return boards, numbers
end

local function checkNumber(board, number)
	local ret = false
	for i,v in ipairs(board) do
		for j,w in ipairs(v) do
			if w.number == number then
				w.marked = true
				if board[i][1].marked and board[i][2].marked and board[i][3].marked and board[i][4].marked and board[i][5].marked then
					ret = true
				elseif board[1][j].marked and board[2][j].marked and board[3][j].marked and board[4][j].marked and board[5][j].marked then
					ret = true
				end
			end
		end
	end
	return ret
end

local function sumUnmarked(b)
	local sum = 0
	for _,v in pairs(b) do
		for _,w in pairs(v) do
			if not w.marked then sum = sum + w.number end
		end
	end
	return sum
end

function _M.part1(file)
	local boards, numbers = parseFile(file)
	for _,n in ipairs(numbers) do
		for _,b in ipairs(boards) do
			if checkNumber(b, n) then
				return sumUnmarked(b)*n
			end
		end
	end
end

function _M.part2(file)
	local boards, numbers = parseFile(file)
	for _,n in ipairs(numbers) do
		-- print("tick number", n)
		local toRemove = {}
		for i,b in ipairs(boards) do
			if checkNumber(b, n) then table.insert(toRemove, 1, i) end
			if #boards - #toRemove == 0 then return sumUnmarked(b)*n end
		end
		for _,i in ipairs(toRemove) do table.remove(boards, i) end
	end
end

return _M
