# CS+ startup python script

ThrowExceptSave = common.ThrowExcept
common.ThrowExcept = True
try:
    if ScriptStarted == True:
        pass
except:
    ScriptStarted = False
common.ThrowExcept = ThrowExceptSave
del ThrowExceptSave

if ScriptStarted == False:
    ScriptStarted = True
    common.Hook(__file__)

def AfterDownload():
    ThrowExceptSave = common.ThrowExcept
    ViewOutputSave  = common.ViewOutput
    common.ThrowExcept = False
    common.ViewOutput  = False
    if debugger.DebugTool.GetType() == DebugTool.Simulator:
        debugger.Memory.Write(debugger.Address('_renesas_simulator_debugging_key'), 0x00000001, MemoryOption.Word)
    common.ThrowExcept = ThrowExceptSave
    common.ViewOutput  = ViewOutputSave
    return
