local utils = require"aocutils"

local _M = {}

local function parse_file(file)
	local players = {}
	for line in file:lines() do
		local tmp = line:match("^Player %d+ starting position: (%d+)$")
		table.insert(players, {pos = tonumber(tmp), score=0})
	end

	return players
end

local function step(rolled, player, players, maxPoints)
	maxPoints = maxPoints or 1000
	local fieldSize = 10

	-- roll
	local active = players[player]

	active.pos = ((active.pos + rolled -1) % fieldSize)+1
	active.score = active.score + active.pos

	return active.score < maxPoints, (player % #players)+1
end

function _M.part1(file)
	local function _dice(i)
		while true do
			coroutine.yield(i)
			i = i + 1
		end
	end
	local function diceGen(i)
		return coroutine.wrap(function() _dice(i) end)
	end

	file = io.open("day21.dat.testing")

	local players = parse_file(file)
	local active  = 1
	local running = true
	local dice    = diceGen(1)
	while running do
		running,active = step(dice()+dice()+dice(), active, players)
	end
	-- only works with two players
	return (dice()-1) * players[active].score
end

local function serialize(t)
	return ("players={1={pos={%d},score={%d}},2={pos={%d},score={%d}}},running={%s},active={%d}"):format(t.players[1].pos, t.players[1].score, t.players[2].pos, t.players[2].score, t.running and "true" or "false", t.active)
	-- if type(t) ~= "table" then return tostring(t) end
 --
	-- local r = {}
	-- for k,v in pairs(t) do
	-- 	table.insert(r, string.format("%s={%s},", k,serialize(v)))
	-- end
	-- return table.concat(r)
end

local function state(t)
	local r = {}
	for k,v in pairs(t) do
		table.insert(r, k .. " ("..tostring(v[2])..")")
	end
	table.sort(r)
	return r
end

function _M.part2(file)
	-- file = io.open("day21.dat.testing")

	local games = {}
	local g = {players=parse_file(file), active=1, running=true}
	games[serialize(g)] = {g, 1}
	local playersWon = {}
	for i,_ in ipairs(g.players) do
		playersWon[i] = 0
	end

	local dices = {
		{3, 1},
		{4, 3},
		{5, 6},
		{6, 7},
		{7, 6},
		{8, 3},
		{9, 1},
	}

	local running = true
	while running do
		running = false
		local games2 = {}
		for s,v in pairs(games) do
			running = true
			local g,num = table.unpack(v)
			for _,d in ipairs(dices) do
				local rolled,numD = table.unpack(d)
				local g_ = utils.table.copy_deep(g)
				local r,a = step(rolled, g_.active, g_.players, 21)
				if r then
					g_.active = a
					local s = serialize(g_)
					if not games2[s] then
						games2[s] = {g_, num*numD}
					else
						games2[s][2] = games2[s][2] + num*numD
					end
				else
					playersWon[g_.active] = playersWon[g_.active] + num*numD
				end
			end
		end
		games = games2
		-- print(table.concat(state(games), "\n"))
		-- print()
	end
	return math.max(playersWon[1], playersWon[2])
end

return _M
