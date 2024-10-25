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
    for op in get_operands(s):
        ret = is_hex_constant(op, in_exe_range)
        if ret is not None:
            return ret   
    return None

def get_constant_from_deref(s, in_exe_range):
    pos = s.find("(")
    if pos != -1:
        tmp = s[:pos]
        if len(tmp) > 0:
            return is_hex_constant(tmp, in_exe_range)
    return None

def get_constant_from_deref_inst_generic(s, in_exe_range):
    for op in get_operands(s):
        ret = get_constant_from_deref(op, in_exe_range)
        if ret is not None:
            return ret   
    return None

def extract_constant(s):
    ret = None
    if s.startswith("movw") or s.startswith("movl") or s.startswith("mov") or s.startswith("cmp"):
        ops = s.split(" ")[1].split(",")
        for op in ops:
            ret = get_constant_from_deref(op, True)
            if ret is not None:
                break
        if ret is None:
            ret = get_constant_from_inst_generic(s, True)
    elif s.startswith("call") or s.startswith("calll"):
        ret = get_constant_from_inst_generic(s, False)
    elif s.startswith("jmpl"):
        ret = get_constant_from_deref(s.split(" ")[1], True)
        if ret is None:
             ret = get_constant_from_inst_generic(s, True)
    elif s.startswith("push") or s.startswith("jmp") or s.startswith("fmull") or s.startswith("fmuls"):
        ret = is_hex_constant(s.split(" ")[1], False)
    elif s.startswith("lea"):
        ret = get_constant_from_deref_inst_generic(s, True)

    return [ret]


def replace_constants(line, constants):
    found_constant = None
    for c in constants:
        # bad time complexity here... but the asm dumps per func are usually small so its fine... for now
        if c in line:
            # find the biggest match and only use that one
            if found_constant is None or len(c) > len(found_constant):
                found_constant = c
    if not found_constant is None:
        line = line.replace(found_constant, constants[found_constant])
    return line

def post_process_asm(asmstr):
    lines = asmstr.split("\n")
    constants = {}
    for line in lines:
       v = extract_constant(line)
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

class TestStringMethods(unittest.TestCase):

    def test_movw_hex_str2(self):
        self.assertEqual(extract_constant("movw $1,0x434A18"), ["0x434A18"])

    def test_mov_hex_str1(self):
        self.assertEqual(extract_constant("mov 0x442F18,%edx"), ["0x442F18"])

    def test_mov_hex_str2(self):
        self.assertEqual(extract_constant("mov %cx,0x434A10"), ["0x434A10"])

    def test_mov_hex_str3(self):
        self.assertEqual(extract_constant("mov $0x42AF10,%edi"), ["$0x42AF10"])

    def test_mov_hex_str4(self):
        self.assertEqual(extract_constant("mov %dl,0x436494(%eax)"), ["0x436494"])

    def test_mov_hex_str5(self):
        self.assertEqual(extract_constant("mov 0x416000(%eax),%cl"), ["0x416000"])

    #def test_mov_hex_str5(self):
    #    self.assertEqual(extract_constant("movl $0x67D818,0x67D704"), ["$0x67D818", "0x67D704"])

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


unittest.main()
