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
    # Prepare an Action Event Object
    ae = ActionEventCondition()
    ae.Address = "sim_debugger_console.c#1"
    ae.Output = "char *"
    ae.Expression = "message"
    ae.ActionEventType = ActionEventType.Printf
    # Search an existing Action Event Object
    ai = None
    for ai in debugger.ActionEvent.Information():
        if ai.Address == ae.Address:
            break;
        else:
            ai = None
    # When it is found, re-create it keeping enabled/disabled to re-evaluate
    # the event address value of it. On the other hand, when it is not found,
    # in case of simulator, create new one ENABLED, but in case of emulator,
    # create new one DISABLED for the safe because Action Event takes a long
    # time period of break state such as hundreds millisecond order.
    if ai != None:
        debugger.ActionEvent.Delete(ai.Number)
    ae_number = debugger.ActionEvent.Set(ae)
    if ai != None:
        if ai.Enable != True:
            debugger.ActionEvent.Disable(ae_number)
    else:
        if debugger.DebugTool.GetType() != DebugTool.Simulator:
            debugger.ActionEvent.Disable(ae_number)
    # Set the switch key variable in case of simulator.
    if debugger.DebugTool.GetType() == DebugTool.Simulator:
        debugger.Watch.SetValue('renesas_simulator_debugging_key', 0x0001)
    common.ThrowExcept = ThrowExceptSave
    common.ViewOutput  = ViewOutputSave
    return

