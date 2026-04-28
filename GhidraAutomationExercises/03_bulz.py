#@runtime PyGhidra


fm = currentProgram.getFunctionManager()
listing = currentProgram.getListing()


def count_instructions(func):
    pass #TODO


for func in fm.getFunctions(True):
    name = "TODO"
    addr = "TODO"
    instr_count = count_instructions(func)
    print(f"{name} @ {addr}: {instr_count} instructions")
