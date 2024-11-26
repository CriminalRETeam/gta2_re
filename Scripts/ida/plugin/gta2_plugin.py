import ida_kernwin
import idc
import idautils
from gta2_tools.stubs_generator import StubsGenerator

try:
    import pyclip
except ImportError:
    ida_kernwin.warning("The python module 'pyclip' is required to use some features of the gta2 plugin.\n\nInstall pyclip and restart IDA.")

def is_target_class_func(ida_func_name: str, old_class_name) -> bool:
    if "::" in ida_func_name:
        class_name = ida_func_name.split("::")[0]
        if class_name == old_class_name:
            return True
    return False

def rename_class(old_class_name: str, new_class_name: str, rename_local_type: bool, rename_functions: bool):
    print(F"renaming {old_class_name} to {new_class_name}")
    if rename_local_type:
        id = idc.get_struc_id(old_class_name)
        if id != -1:
            idc.set_struc_name(id, new_class_name)
        else:
            print(f"could not find local type: {old_class_name}")

    if rename_functions:
        for segment in idautils.Segments():
            for ea in idautils.Functions(segment, idc.get_segm_end(segment)):
                ida_func_name = idc.get_func_name(ea)
                if is_target_class_func(ida_func_name, old_class_name):
                    func_name = ida_func_name.split("::")[1]
                    idc.set_name(ea, f"{new_class_name}::{func_name}")

class rename_class_form_t(ida_kernwin.Form):

    def __init__(self):
        self.invert = False
        F = ida_kernwin.Form
        F.__init__(
            self,
            r"""STARTITEM {id:rRenameLocalType}
BUTTON YES* Rename
BUTTON CANCEL Cancel
Rename C++ Class

{FormChangeCb}

<#Hint1#Current Class Name  :{iStr1}>
<#Hint1#New Class Name      :{iStr2}>

<##Options##Rename Local Type:{rRenameLocalType}>
<Rename Functions:{rRenameFunctions}>{cGroup1}>

""", {
            'iStr1': F.StringInput(),
            'iStr2': F.StringInput(),
            'cGroup1': F.ChkGroupControl(("rRenameLocalType", "rRenameFunctions")),
            'FormChangeCb': F.FormChangeCb(self.OnFormChange),
        })

    def OnFormChange(self, fid):
        if fid == self.iStr1.id:
            print(f"string changed to {self.iStr1.value}")
        return 1

    @staticmethod
    def open():
        f = rename_class_form_t()

        # Compile (in order to populate the controls)
        f.Compile()

        f.rRenameLocalType.checked = True
        f.rRenameFunctions.checked = True

        # Execute the form
        ok = f.Execute()
        if ok == 1:
            rename_class(f.iStr1.value, f.iStr2.value, f.rRenameLocalType.checked, f.rRenameFunctions.checked)

        # Dispose the form
        f.Free()

class class_rename_t(ida_kernwin.action_handler_t):
    def __init__(self):
        ida_kernwin.action_handler_t.__init__(self)

    def activate(self, ctx):
        rename_class_form_t.open()

    def update(self, ctx):
        return ida_kernwin.AST_ENABLE_ALWAYS


class stub_class_form_t(ida_kernwin.Form):

    def __init__(self):
        self.invert = False
        F = ida_kernwin.Form
        F.__init__(
            self,
            r"""STARTITEM {id:className}
Stub C++ Class

{FormChangeCb}
<#Hint1#Class Name  :{className}>
<##Header Options##Wrap In Class:{rWrapInClass}>
<Include Functions:{rIncludeFunctions}>
<Include Fields:{rIncludeFields}>{cGroup1}>
<##Copy Header To Clipboard:{copyHeaderButton}> <##Copy Source To Clipboard:{copySourceButton}>

""", {
            'className': F.StringInput(),
            'copySourceButton': F.ButtonInput(self.OnSourceButton),
            'copyHeaderButton': F.ButtonInput(self.OnHeaderButton),
            'cGroup1': F.ChkGroupControl(("rWrapInClass", "rIncludeFunctions", "rIncludeFields")),
            'FormChangeCb': F.FormChangeCb(self.OnFormChange),
        })

    def generate_stubs(self, stubs_type: StubsGenerator.StubsType):
        class_name = self.GetControlValue(self.className)
        print(f"CLASS INPUT IS: {class_name}, type is: {type(class_name)}, REPR: {repr(class_name)}")
        
        if class_name is None or class_name == "":
            ida_kernwin.warning("Class name can't be empty")
            return
        
        generator = StubsGenerator(self.GetControlValue(self.rWrapInClass), self.GetControlValue(self.rIncludeFunctions), self.GetControlValue(self.rIncludeFields))
        stubs_str = generator.generate_stubs(class_name, stubs_type)
        if  stubs_str is None:
            ida_kernwin.warning("Class not found or there was nothing to stub")
            return
        
        pyclip.copy(stubs_str)
        ida_kernwin.info("Successfully copied to clipboard")

    def OnSourceButton(self, code=0):
        self.generate_stubs(StubsGenerator.StubsType.Source)

    def OnHeaderButton(self, code=0):
        self.generate_stubs(StubsGenerator.StubsType.Header)

    def OnFormChange(self, fid):
        if fid == self.className.id:
            pass
        return 1

    @staticmethod
    def open():
        f = stub_class_form_t()

        # Compile (in order to populate the controls)
        f.Compile()

        f.rWrapInClass.checked = True
        f.rIncludeFunctions.checked = True
        f.rIncludeFields.checked = True

        # Execute the form
        ok = f.Execute()

        # Dispose the form
        f.Free()



class stub_class_t(ida_kernwin.action_handler_t):
    def __init__(self):
        ida_kernwin.action_handler_t.__init__(self)

    def activate(self, ctx):
        stub_class_form_t.open()

    def update(self, ctx):
        return ida_kernwin.AST_ENABLE_ALWAYS

ida_kernwin.create_menu("GTA2ToolsMenu", "GTA2 Tools", "gta2 tools")

desc1 = ida_kernwin.action_desc_t("rename_action", "Rename C++ Class", class_rename_t())
ida_kernwin.register_action(desc1)
ida_kernwin.attach_action_to_menu("gta2 tools", "rename_action", ida_kernwin.SETMENU_INS)

desc2 = ida_kernwin.action_desc_t("stub_class_action", "Stub C++ Class", stub_class_t())
ida_kernwin.register_action(desc2)
ida_kernwin.attach_action_to_menu("gta2 tools", "stub_class_action", ida_kernwin.SETMENU_INS)

