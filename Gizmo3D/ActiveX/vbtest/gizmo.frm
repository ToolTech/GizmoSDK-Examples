VERSION 5.00
Object = "{73511247-BE5B-4605-A7BB-A82D215BFBAB}#1.0#0"; "ActiveX.dll"
Begin VB.Form Form1 
   Caption         =   "Gizmo3D VB example"
   ClientHeight    =   5880
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   7710
   LinkTopic       =   "Form1"
   ScaleHeight     =   5880
   ScaleWidth      =   7710
   StartUpPosition =   3  'Windows Default
   Begin VB.Timer Timer1 
      Interval        =   10
      Left            =   3600
      Top             =   5760
   End
   Begin VB.ListBox List1 
      Height          =   1815
      Left            =   0
      TabIndex        =   0
      Top             =   3960
      Width           =   7575
   End
   Begin ACTIVEXLibCtl.gzWindowCom gzWindowCom1 
      CausesValidation=   0   'False
      DragMode        =   1  'Automatic
      Height          =   4335
      Left            =   0
      OleObjectBlob   =   "gizmo.frx":0000
      TabIndex        =   1
      Top             =   0
      Width           =   7575
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim camera As New gzCameraCom
Dim scene As New gzSceneCom
Dim WithEvents messenger As gzMessageCom
Attribute messenger.VB_VarHelpID = -1
Dim radius, a, b, sa, sb, pi As Single

Private Sub Form_Load()

gzWindowCom1.activateGizmoWindow

radius = 50
a = 0
b = 0
sa = 0.01
sb = 0.001
pi = 3.14159265


Set messenger = New gzMessageCom
camera.setScene scene

scene.useAutoScale 1
scene.setScale 10

scene.loadURL "./gizmo.3ds"
gzWindowCom1.setCamera camera
gzWindowCom1.setRefreshRate 60

camera.setNearFar 1, 1000


camera.setPosition 50, 50, 120
camera.lookAt 0, 0, 0



End Sub

Private Sub Form_Resize()

    gzWindowCom1.Width = Width - 100
    gzWindowCom1.Height = Height - 400 - List1.Height
    gzWindowCom1.Top = 0
    gzWindowCom1.Left = 0
    
    List1.Width = Width - 100
    List1.Top = gzWindowCom1.Height
    
End Sub

Private Sub messenger_onMessage(ByVal message As String, ByVal sender As String, ByVal level As Long)
List1.AddItem message
End Sub

Sub calculateCameraPosition()

camera.setPosition radius * Cos(a) * Cos(b), radius * Sin(a) * Cos(b), radius * Sin(b)

camera.lookAt 0, 0, 0

End Sub


Private Sub Timer1_Timer()
a = a + sa
b = b + sb

If a > (2 * pi) Then a = a - 2 * pi

If b > (pi / 2) Then
    b = pi / 2
    sb = -sb
End If

If b < 0 Then
    b = 0
    sb = -sb
End If

calculateCameraPosition

End Sub
