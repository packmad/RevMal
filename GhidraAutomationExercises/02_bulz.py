#@runtime PyGhidra


def read_c_string(start_addr):
    pass #TODO


def decrypt_str(enc_string_addr, key_addr, length):
    key = read_c_string(key_addr)
    pass #TODO


enc_str_addr = toAddr(0x10002034)
key_addr = toAddr(0x10002020)
decrypted_str = decrypt_str(enc_str_addr, key_addr, 0x10).decode('utf-8').rstrip('\x00')

print(decrypted_str)
