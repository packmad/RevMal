#@runtime PyGhidra

from ghidra.program.model.scalar import Scalar


def get_references_to_decrypt_function():
    pass #TODO


def get_push_immediate(inst):
    pass #TODO


def read_c_string(start_addr):
    pass #TODO


def decrypt_str(enc_string_addr, key_addr, length):
    pass #TODO


for xref in get_references_to_decrypt_function():
    pass #TODO
