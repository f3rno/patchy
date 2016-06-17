ldi a, 0
ldi b, 1
ldi c, 16 # nth fib number
ldi f, [done]
ldi dp, [loop] # Use DP for 2nd label, lazy implementation

xor d, d  # result
xor e, e  # i

# Handle first two fib numbers (0, 1) manually
cmp c, e
je f

cmp c, b
ldi d, 1
je f

# Loop for the others, dec c to take starting conditions into account
subi c, 1

loop:
  add a, b
  mov d, a
  addi e, 1
  cmp c, e
  je f

  add b, a
  mov d, b
  addi e, 1
  cmp c, e
  jne dp

done:
  hlt
