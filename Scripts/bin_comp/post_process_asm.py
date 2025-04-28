import unittest

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

def get_operands(s):
    return s.split(" ")[1].split(",")

def is_hex_constant(s, in_exe_range):
    tmp = s
    if tmp[0] == "$" or tmp[0] == "*":
        tmp = tmp[1:]
    if in_exe_range:
        if is_hex_str_in_exe_range(tmp):
            return s
    else:
        if is_hex_str(tmp):
            return s
    return None

def get_constant_from_inst_generic(s, in_exe_range):
    r = []
    for op in get_operands(s):
        ret = is_hex_constant(op, in_exe_range)
        if ret is not None:
            r.append(ret) 
    return r

def get_constant_from_deref(s, in_exe_range):
    pos = s.find("(")
    if pos != -1:
        tmp = s[:pos]
        if len(tmp) > 0:
            return is_hex_constant(tmp, in_exe_range)
    return None

def get_constant_from_deref_inst_generic(s, in_exe_range):
    r = []
    for op in get_operands(s):
        ret = get_constant_from_deref(op, in_exe_range)
        if ret is not None:
            r.append(ret) 
    return r

def extract_constant(s):
    ret = []
    if s.startswith("movw") or s.startswith("movl") or s.startswith("mov") or s.startswith("cmp") or s.startswith("imul"):
        ops = s.split(" ")[1].split(",")
        for op in ops:
            tmp = get_constant_from_deref(op, True)
            if tmp is not None:
                ret.append(tmp)
                break
        if len(ret) == 0:
            ret = get_constant_from_inst_generic(s, True)
    elif s.startswith("call") or s.startswith("calll"):
        ret = get_constant_from_inst_generic(s, False)
    elif s.startswith("jmpl") or s.startswith("sub") or s.startswith("fstps"):
        tmp = get_constant_from_deref(s.split(" ")[1], True)
        if tmp is None:
             ret = get_constant_from_inst_generic(s, True)
        else:
            ret.append(tmp)
    elif s.startswith("push") or s.startswith("jmp") or s.startswith("fmull") or s.startswith("fmuls"):
        tmp = is_hex_constant(s.split(" ")[1], False)
        if tmp is not None:
            ret.append(tmp)
    elif s.startswith("lea"):
        ret = get_constant_from_deref_inst_generic(s, True)
    elif s.startswith("inc"):
        ret = get_constant_from_inst_generic(s, True)
    elif s.startswith("dec"):
        ret = get_constant_from_inst_generic(s, True)

    return ret


def replace_constants(line, constants):
    # Create a copy of the constants dictionary
    constants_copy = constants.copy()
    
    while constants_copy:
        found_constant = None
        for c in constants_copy:
            # Bad time complexity here, but for small ASM dumps per function, it's manageable
            if c in line:
                # Find the biggest match and only use that one
                if found_constant is None or len(c) > len(found_constant):
                    found_constant = c
        if found_constant:
            line = line.replace(found_constant, constants_copy[found_constant])
            # Remove the replaced constant from the copy to prevent looping
            del constants_copy[found_constant]
        else:
            return line
    return line


def post_process_asm(asmstr):
    stable_name_index = 1
    lines = asmstr.split("\n")
    constants = {}
    for line in lines:
       extracted_constants = extract_constant(line)
       for v in extracted_constants:
            if not v in constants:
                constants[v] = "stable_name_" + str(stable_name_index)
                stable_name_index = stable_name_index + 1
    i = 0
    while i < len(lines):
        lines[i] = replace_constants(lines[i], constants)
        i = i + 1

    #for line in lines:
    #    print(line)
    
    #for c in constants:
    #    print(c + " : " + constants[c])

    return "\n".join(lines)

class TestStringMethods(unittest.TestCase):

    def test_movw_hex_str2(self):
        self.assertEqual(extract_constant("movw $1,0x434A18"), ["0x434A18"])

    def test_mov_hex_str1(self):
        self.assertEqual(extract_constant("mov 0x442F18,%edx"), ["0x442F18"])

    def test_imul_hex(self):
        self.assertEqual(extract_constant("imul 0x706338,%eax"), ["0x706338"])

    def test_mov_hex_str2(self):
        self.assertEqual(extract_constant("mov %cx,0x434A10"), ["0x434A10"])

    def test_mov_hex_str3(self):
        self.assertEqual(extract_constant("mov $0x42AF10,%edi"), ["$0x42AF10"])

    def test_mov_hex_str4(self):
        self.assertEqual(extract_constant("mov %dl,0x436494(%eax)"), ["0x436494"])

    def test_mov_hex_str5(self):
        self.assertEqual(extract_constant("mov 0x416000(%eax),%cl"), ["0x416000"])

    def test_mov_hex_str5(self):
        self.assertEqual(extract_constant("movl $0x67D818,0x67D704"), ["$0x67D818", "0x67D704"])

    def test_call_hex(self):
        self.assertEqual(extract_constant("call 0x00013BED"), ["0x00013BED"])

    def test_calll_hex(self):
        self.assertEqual(extract_constant("calll *0x426208"), ["*0x426208"])

    def test_push_hex(self):
        self.assertEqual(extract_constant("push $0x42A3A4"), ["$0x42A3A4"])

    def test_jmpl_hex1(self):
        self.assertEqual(extract_constant("jmpl *0x40FC68(,%eax,4)"), ["*0x40FC68"])

    def test_jmpl_hex2(self):
        self.assertEqual(extract_constant("jmpl *0x43656C"), ["*0x43656C"])

    def test_jmp_hex(self):
        self.assertEqual(extract_constant("jmp 0xFFFFFC60"), ["0xFFFFFC60"])

    def test_lea_hex1(self):
        self.assertEqual(extract_constant("lea 0x4262B8(,%ecx,2),%edi"), ["0x4262B8"])

    def test_lea_hex2(self):
        self.assertEqual(extract_constant("lea (%ecx,%eax,8),%eax"), [])

    def test_lea_hex2(self):
        self.assertEqual(extract_constant("cmp %bl,0x43D0F7"), ["0x43D0F7"])

    def test_fmull_hex(self):
        self.assertEqual(extract_constant("fmull 0x427F00"), ["0x427F00"])

    def test_fmuls_hex(self):
        self.assertEqual(extract_constant("fmuls 0x5FE3C8"), ["0x5FE3C8"])

    def test_inc_hex(self):
        self.assertEqual(extract_constant("inc 0x4D6BAC"), ["0x4D6BAC"])

    def test_incw_hex(self):
        self.assertEqual(extract_constant("incw 0x4D6BAC"), ["0x4D6BAC"])

    def test_incl_hex(self):
        self.assertEqual(extract_constant("incl 0x4D6BAC"), ["0x4D6BAC"])

    def test_dec_hex(self):
        self.assertEqual(extract_constant("dec 0x4D6BAC"), ["0x4D6BAC"])

    def test_decw_hex(self):
        self.assertEqual(extract_constant("decw 0x4D6BAC"), ["0x4D6BAC"])

    def test_decl_hex(self):
        self.assertEqual(extract_constant("decl 0x4D6BAC"), ["0x4D6BAC"])

    def test_sub_hex(self):
        self.assertEqual(extract_constant("sub 0x4E1A58(%eax),%dx"), ["0x4E1A58"])
        self.assertEqual(extract_constant("sub $0x46B058,%eax"), ["$0x46B058"])

    def test_fstps_hex(self):
        self.assertEqual(extract_constant("fstps 0x6F65A8(%eax)"), ["0x6F65A8"])

#unittest.main()
