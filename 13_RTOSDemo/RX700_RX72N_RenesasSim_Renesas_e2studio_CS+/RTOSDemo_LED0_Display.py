import clr
clr.AddReference("System.Windows.Forms")
clr.AddReference("System.Drawing")

from System.Windows.Forms import Application, Form, FormBorderStyle, Label, Button, Timer
from System.Drawing import Size, Color, Font, FontStyle, ContentAlignment

class IForm(Form):
    def __init__(self):
        self.Text = 'RTOS Demo LED0 Display Form'
        self.ClientSize = Size(440, 100)
        self.FormBorderStyle = FormBorderStyle.FixedSingle
        self.MaximizeBox = False
        font = Font('MS UI Gothic', 32, FontStyle.Bold)
        ta = ContentAlignment.MiddleCenter
        self.LED0 = Label(Top=20, Left=20, Height=60, Width=120, Font=font, TextAlign=ta)
        self.LED0.Text = 'LED0'
        self.LED0.ForeColor = Color.White
        self.Controls.Add(self.LED0)
        self.timer = Timer()
        self.timer.Interval = 200
        self.timer.Tick += self.OnTickTimer
    def OnLoad(self, event):
        print 'Now Running...'
        self.ThrowExceptSave = common.ThrowExcept
        self.ViewOutputSave  = common.ViewOutput
        common.ThrowExcept = False
        common.ViewOutput  = False
        self.timer.Start()
        self.BringToFront()
    def OnClosed(self, event):
        self.timer.Stop()
        common.ThrowExcept = self.ThrowExceptSave
        common.ViewOutput  = self.ViewOutputSave
        print 'Quit'
    def OnTickTimer(self, sender, event):
        # The debugger (or build) object does not exist if no project is opened.
        # Or the debugger (or build) object will disappear after project is closed.
        try:
            if 0 == debugger.Watch.GetValue("PORT4.PODR.B0"):
                self.LED0.BackColor = Color.Blue
            else:
                self.LED0.BackColor = Color.DarkGray
        except:
            pass

Application.Run(IForm())
