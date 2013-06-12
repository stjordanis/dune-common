dofile( "tupleutility.lua" )

local tuple_length = 9
indent = "  "

print( indent .. "// tuple_push_back" )
print( indent .. "// ---------------" )

print ""

for i = 1, tuple_length do
  body = tuple_push_back( i, indent )
  print( body )
  print ""
end

print ""
print ""

print( indent .. "// tuple_push_front" )
print( indent .. "// ----------------" )

print ""

for i = 1, tuple_length do
  body = tuple_push_front( i, indent )
  print( body )
  print ""
end

print ""
print ""

print( indent .. "// tuple_pop_back" )
print( indent .. "// --------------" )

print ""

for i = 1, tuple_length do
  body = tuple_pop_back( i, indent )
  print( body )
  print ""
end

print ""
print ""

print( indent .. "// tuple_pop_front" )
print( indent .. "// ---------------" )

print ""

for i = 1, tuple_length do
  body = tuple_pop_front( i, indent )
  print( body )
  print ""
end
