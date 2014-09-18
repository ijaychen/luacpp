
function f(tb, x, y) 
	return x + y + tb.n
end

b={}
b.__call = f
a={}
a.n=100
setmetatable(a,b)
print(a(1,2))
print(a.__index)

print("----------------------------")

local function metatable()
	t = {}
	print(getmetatable(t))
	t1 = {}
	setmetatable(t, t1)
	assert(getmetatable(t)==t1)
	print(getmetatable(t))
	print(t1)
end

metatable()
