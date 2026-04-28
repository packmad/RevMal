#@runtime PyGhidra


def read_c_string(start_addr):
    pass #TODO


addr = toAddr(0x10002124)
c_str = read_c_string(addr).decode('utf-8').rstrip('\x00')
print("String at 0x10002124:", c_str)
