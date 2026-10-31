import re
import sys
from pathlib import Path

REPO_ROOT_FOLDER = Path(__file__).parent.parent.parent
SOURCE_FOLDER = REPO_ROOT_FOLDER / "Source"

class globals():
    __slots__ = "var_type", "name", "address", "source_file", "src_line_num", "unique_name"

    def __init__(self, var_type : str, name : str, address : int, source_file : str, src_line_num : int):
        self.var_type = var_type
        self.name = name
        self.address = address
        self.source_file = source_file
        self.src_line_num = src_line_num
        self.unique_name = True


# macro: (argument count, index of the name, whether the name must be unique). Statics may share a name with
# statics of other files or functions, but never an address.
MACROS = {
    "DEFINE_GLOBAL": (3, 1, True),
    "DEFINE_GLOBAL_INIT": (4, 1, True),
    "DEFINE_STATIC_GLOBAL": (3, 1, False),
    "DEFINE_STATIC_GLOBAL_INIT": (4, 1, False),
    "DEFINE_STATIC_MEMBER": (4, 2, False),
    "DEFINE_STATIC_MEMBER_INIT": (5, 2, False),
    "LOCAL_STATIC": (3, 1, False),
    "LOCAL_STATIC_INIT": (4, 1, False),
}


class GlobalParseError(ValueError):
    pass


def mask_comments_and_literals(text, filename):
    """Keep source offsets/newlines while hiding tokens inside comments or quotes."""
    clean = list(text)
    code = list(text)
    index = 0

    def mask(chars, start, end):
        for position in range(start, end):
            if chars[position] != "\n":
                chars[position] = " "

    while index < len(text):
        start = index
        if text.startswith("//", index):
            end = text.find("\n", index)
            end = len(text) if end == -1 else end
            mask(clean, start, end)
            mask(code, start, end)
        elif text.startswith("/*", index):
            end = text.find("*/", index + 2)
            if end == -1:
                line = text.count("\n", 0, start) + 1
                raise GlobalParseError(f"{filename}({line}): unterminated block comment")
            end += 2
            mask(clean, start, end)
            mask(code, start, end)
        elif text[index] in ('"', "'"):
            quote = text[index]
            end = index + 1
            while end < len(text):
                if text[end] == "\\":
                    end += 2
                elif text[end] == quote:
                    end += 1
                    break
                else:
                    end += 1
            else:
                line = text.count("\n", 0, start) + 1
                raise GlobalParseError(f"{filename}({line}): unterminated quoted literal")
            mask(code, start, end)
        else:
            index += 1
            continue
        index = end
    return "".join(clean), "".join(code)


def iter_globals(text, filename):
    clean, code = mask_comments_and_literals(text, filename)
    # Arrays remain deliberately excluded. Match whole macro names only.
    macro_pattern = re.compile(r"\b(" + "|".join(sorted(MACROS, key=len, reverse=True)) + r")\s*\(")
    position = 0
    while match := macro_pattern.search(code, position):
        macro = match[1]
        line = text.count("\n", 0, match.start()) + 1
        argument_start = match.end()
        index = argument_start
        depth = 1
        arguments = []
        while index < len(code) and depth:
            char = code[index]
            if char == "(":
                depth += 1
            elif char == ")":
                depth -= 1
                if depth == 0:
                    arguments.append(clean[argument_start:index].strip())
                    break
            elif char == "," and depth == 1:
                arguments.append(clean[argument_start:index].strip())
                argument_start = index + 1
            index += 1
        if depth:
            raise GlobalParseError(f"{filename}({line}): unterminated {macro} invocation")

        expected, name_index, unique_name = MACROS[macro]
        if len(arguments) != expected or not all(arguments):
            raise GlobalParseError(f"{filename}({line}): {macro} expects {expected} nonempty arguments, got {len(arguments)}")
        if not re.fullmatch(r"0[xX][0-9a-fA-F]+", arguments[-1]):
            raise GlobalParseError(f"{filename}({line}): invalid global address {arguments[-1]!r}")
        declaration = globals(arguments[0], arguments[name_index], int(arguments[-1], 16), filename, line)
        declaration.unique_name = unique_name
        yield declaration
        position = index + 1


def main():
    if not SOURCE_FOLDER.exists():
        print(f"Path not found: {str(SOURCE_FOLDER)}")
        sys.exit(1)

    globals_list = list()
    error_list = list()

    for src_file in SOURCE_FOLDER.iterdir():
        path = str(src_file)

        # exclude folders and other unwanted files  (no define globals in headers)
        # _tu_harness_tmp*.cpp: transient copies made by Scripts/tu_harness/tu.sh
        if path.endswith(".cpp") and not src_file.name.startswith("_tu_harness_tmp"):
            try:
                declarations = list(iter_globals(src_file.read_text(), src_file.name))
            except GlobalParseError as error:
                print(error)
                sys.exit(2)

            for curr_global in declarations:
                duplicated = False
                for global_iter in globals_list:
                    same_name = global_iter.name == curr_global.name and global_iter.unique_name and curr_global.unique_name
                    if global_iter.address == curr_global.address or same_name:
                        # duplicated global
                        duplicated = True
                        error_list.append((global_iter, curr_global))

                if not duplicated:
                    globals_list.append(curr_global)

    if len(error_list) == 0:
        sys.exit(0)
    else:
        for global_1, global_2 in error_list:
            print(f"Found duplicate: {global_1.name} on {global_1.source_file}({global_1.src_line_num}) and {global_2.name} on {global_2.source_file}({global_2.src_line_num})")
        sys.exit(1)


if __name__ == "__main__":
    main()
