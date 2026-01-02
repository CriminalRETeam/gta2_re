import sys
from pathlib import Path

REPO_ROOT_FOLDER = Path(__file__).parent.parent.parent
SOURCE_FOLDER = REPO_ROOT_FOLDER / "Source"

class globals():
    __slots__ = "var_type", "name", "address", "source_file"

    def __init__(self, var_type : str, name : str, address : int, source_file : str):
        self.var_type = var_type
        self.name = name
        self.address = address
        self.source_file = source_file

upper_hex = lambda hex_str : hex_str[2:].upper()

def get_uninit_global(line : str, filename : str):

    line = line.strip()
    parenthesis_span = slice(line.find('(') + 1, line.rfind(')'))
    arguments = line[parenthesis_span].split(",")

    if len(arguments) > 3:
        print(f"More than 3 arguments in DEFINE_GLOBAL: {arguments}")
        sys.exit(2)
    elif len(arguments) < 3:
        print(f"Less than 3 arguments in DEFINE_GLOBAL: {arguments}")
        sys.exit(2)

    global_type = arguments[0].strip()
    global_name = arguments[1].strip()
    global_address = int(upper_hex(arguments[2].strip()), 16)

    return globals(global_type, global_name, global_address, filename)
    
def get_inited_global(line : str, filename : str):

    line = line.strip()
    parenthesis_span = slice(line.find('(') + 1, line.rfind(')'))

    arguments = line[parenthesis_span].split(",")

    global_type = arguments[0].strip()
    global_name = arguments[1].strip()
    global_address = int(upper_hex(arguments[-1].strip()), 16)
    
    return globals(global_type, global_name, global_address, filename)


def main():

    if not SOURCE_FOLDER.exists():
        print(f"Path not found: {str(SOURCE_FOLDER)}")
        sys.exit(1)

    globals_list = list()
    error_list = list()

    for src_file in SOURCE_FOLDER.iterdir():
        path = str(src_file)

        # exclude folders and other unwanted files  (no define globals in headers)
        if path.endswith(".cpp"):
            
            with open(path, "r") as file:
                for line in file:

                    curr_global = None
                    
                    if "//" in line:    # remove comments
                        line = line[: line.find("//")]
                    
                    if "DEFINE_GLOBAL_ARRAY" in line.upper():   # TODO: not implemented
                        continue
                    if "DEFINE_GLOBAL_INIT" in line.upper():
                        curr_global = get_inited_global(line, src_file.name)
                    elif "DEFINE_GLOBAL" in line.upper():
                        curr_global = get_uninit_global(line, src_file.name)
                    else:
                        continue
                    
                    if curr_global is not None:
                        duplicated = False
                        for global_iter in globals_list:
                            if global_iter.address == curr_global.address or global_iter.name == curr_global.name:
                                # duplicated global
                                duplicated = True
                                error_list.append((global_iter, curr_global))
                        
                        if not duplicated:
                            globals_list.append(curr_global)

    if len(error_list) == 0:
        sys.exit(0)
    else:
        for global_1, global_2 in error_list:
            print(f"Found duplicate: {global_1.name} on {global_1.source_file} and {global_2.name} on {global_2.source_file}")
        sys.exit(1)


if __name__ == "__main__":
    main()