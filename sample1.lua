require "print_r"
function c_test()
	local obj = A()
	obj:show()
	print("get:", obj:get())
	print("set:", obj:set(11))
end

function c_testInt()
	local val = 1314
	return val
end

function c_testStr()
	local str = "hello world"
	return str
end


function c_testTable(arg)
	print(arg)
	--print_r(arg)
	print(arg.name)
end

function return_table(arg)
	local ret = {}
	ret.name = arg
	return ret
end

function c_initObj(obj)
	local ret = {}
	ret.__gc = print("-----------------------------------------------------ret gc")
	ret.name = "hello world!!!!!"
	print(ret)
	obj:bind(ret)
end
