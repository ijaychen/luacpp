
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
