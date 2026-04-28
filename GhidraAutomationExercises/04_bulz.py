#@runtime PyGhidra


def get_references_to_decrypt_function():
    pass #TODO


xrefs = get_references_to_decrypt_function()
for xref in xrefs:
    print(hex(xref.getFromAddress().getOffset()))
