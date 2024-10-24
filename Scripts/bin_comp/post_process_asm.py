
def load_file(filename):
    with open(filename) as file:
        lines = [line.rstrip() for line in file]
        return lines
    return []

def is_hex_str(line):
    try:
        num = int(line, 16)
        return True
    except:
        return False

def is_hex_str_in_exe_range(line):
    try:
        return int(line, 16) > 0x400000
    except:
        return False

def constant_from_mov(line):
    if line.startswith("mov"):
        parts = line.split(" ")
        parts = parts[1].split(",")
        if parts[0][0] == "%" or parts[0][0] == "$":
            if (is_hex_str_in_exe_range(parts[1])):
                return parts[1]
        else:
            if parts[1][0] == "%" or parts[1][0] == "$":
                if (is_hex_str_in_exe_range(parts[0])):
                    return parts[0]
    return None

def constant_from_push(line):
    if line.startswith("push $"):
        c = line[6:]
        if (is_hex_str_in_exe_range(c)):
            return c
    return None

def constant_from_call(line):
    if line.startswith("call "):
        parts = line.split(" ")
        if (is_hex_str(parts[1])):
            return parts[1]
    return None  

def do_tests():
    if constant_from_mov("mov %cx,0x434A10") != "0x434A10":
        return False
    if constant_from_mov("mov %cx,meh") != None:
        return False
    
    # todo: a lot more test cases for these
    # movw $1,0x434A18
    # push $0x42A3A4 constant_from_push
    # mov 0x442F18,%edx

    if constant_from_call("call 0x00013BED") != "0x00013BED":
        return False

    if constant_from_call("call rozza") != None:
        return False

    return True

def constant_from_statement(line):
    v = constant_from_mov(line)
    if v is not None:
        return v

    v = constant_from_call(line)
    if v is not None:
        return v

    v = constant_from_push(line)
    if v is not None:
        return v

    return None

def replace_constants(line, constants):
    for c in constants:
        if c in line:
            line = line.replace(c, constants[c])
            return line
    return line

def post_process_asm(asmstr):
    lines = asmstr.split("\n")
    constants = {}
    for line in lines:
       v = constant_from_statement(line)
       if v is not None:
        if not v in constants:
            constants[v] = "stable_name"

    i = 0
    while i < len(lines):
        lines[i] = replace_constants(lines[i], constants)
        i = i + 1

    #for line in lines:
    #    print(line)

    #for c in constants:
    #    print(c + " : " + constants[c])
    return "\n".join(lines)

def run_tests():
    if do_tests():
        print("OK!")
    else:
        print("IN THE BIN!!")

#run_tests()
#post_process_asm("Map_0x370::ctor_4E9660_new_asm.txt")
#post_process_asm("Map_0x370::dtor_4E9770_new_asm.txt")
