start:
ldr w0, base_address
ldr w1, pin_offset
str w1, [w0, #0x08]
ldr w2, counter
loop:
ldr w1, setting_address
str w1, [w0, #0x1c]
eor w10, w10, w10
delay1:
add w10, w10, #1
cmp w10, w2
b.ne delay1
ldr w1, setting_address
str w1, [w0, #0x28]
eor w10, w10, w10
delay2:
add w10, w10, #1
cmp w10, w2
b.ne delay2
b loop
base_address:
.int 0x3f200000
pin_offset:
.int 0x8
setting_address:
.int 0x200000
counter:
.int 0x0500000