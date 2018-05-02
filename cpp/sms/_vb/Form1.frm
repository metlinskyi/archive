VERSION 5.00
Object = "{48E59290-9880-11CF-9754-00AA00C00908}#1.0#0"; "MSINET.OCX"
Object = "{EAB22AC0-30C1-11CF-A7EB-0000C05BAE0B}#1.1#0"; "SHDOCVW.dll"
Begin VB.Form Form1 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "SMS"
   ClientHeight    =   825
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   6825
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   55
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   455
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   WhatsThisHelp   =   -1  'True
   Begin VB.CommandButton cmdSend 
      Caption         =   "Send"
      Height          =   285
      Left            =   60
      TabIndex        =   10
      Top             =   390
      Width           =   615
   End
   Begin VB.TextBox txtMsg 
      Height          =   285
      Left            =   3210
      TabIndex        =   9
      Text            =   "Text+of+message"
      Top             =   60
      Width           =   3525
   End
   Begin VB.TextBox txtNumber 
      Height          =   285
      Left            =   720
      TabIndex        =   6
      Text            =   "xxxxxxx"
      Top             =   60
      Width           =   1485
   End
   Begin VB.TextBox txtInterval 
      Height          =   285
      Left            =   720
      TabIndex        =   5
      Text            =   "3600"
      Top             =   1290
      Width           =   975
   End
   Begin VB.CommandButton cmdTimer 
      Caption         =   "Timer"
      Height          =   285
      Left            =   60
      TabIndex        =   4
      Top             =   1290
      Width           =   615
   End
   Begin VB.TextBox txtCX 
      Height          =   285
      Left            =   720
      TabIndex        =   3
      Text            =   "10"
      Top             =   960
      Width           =   975
   End
   Begin VB.Timer tmrSend 
      Enabled         =   0   'False
      Interval        =   3600
      Left            =   4950
      Top             =   1110
   End
   Begin SHDocVwCtl.WebBrowser WebBrowser1 
      Height          =   525
      Left            =   5460
      TabIndex        =   2
      Top             =   1080
      Visible         =   0   'False
      Width           =   645
      ExtentX         =   1138
      ExtentY         =   926
      ViewMode        =   0
      Offline         =   0
      Silent          =   0
      RegisterAsBrowser=   0
      RegisterAsDropTarget=   1
      AutoArrange     =   0   'False
      NoClientEdge    =   0   'False
      AlignLeft       =   0   'False
      NoWebView       =   0   'False
      HideFileNames   =   0   'False
      SingleClick     =   0   'False
      SingleSelection =   0   'False
      NoFolders       =   0   'False
      Transparent     =   0   'False
      ViewID          =   "{0057D0E0-3573-11CF-AE69-08002B2E1262}"
      Location        =   "http:///"
   End
   Begin InetCtlsObjects.Inet inet 
      Left            =   6150
      Top             =   1050
      _ExtentX        =   1005
      _ExtentY        =   1005
      _Version        =   393216
      Protocol        =   4
      URL             =   "http://"
   End
   Begin VB.TextBox txtURL 
      Enabled         =   0   'False
      Height          =   285
      Left            =   720
      TabIndex        =   1
      Text            =   "http://mobile.alkar.net/sms.php?oper=050&number=2904509&text=-VIRUS+OF+LOVE-&sendform=%A0%CE%F2%EF%F0%E0%E2%E8%F2%FC%A0"
      Top             =   390
      Width           =   6015
   End
   Begin VB.CommandButton cmdLoop 
      Caption         =   "Loop"
      Height          =   285
      Left            =   60
      TabIndex        =   0
      Top             =   960
      Width           =   615
   End
   Begin VB.Line Line1 
      BorderWidth     =   2
      X1              =   6
      X2              =   449
      Y1              =   50
      Y2              =   51
   End
   Begin VB.Label Label2 
      Caption         =   "Message"
      Height          =   225
      Left            =   2430
      TabIndex        =   8
      Top             =   90
      Width           =   735
   End
   Begin VB.Label Label1 
      Caption         =   "Phone"
      Height          =   195
      Left            =   90
      TabIndex        =   7
      Top             =   90
      Width           =   585
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim cx As Integer

Private Sub cmdLoop_Click()
    cx = 0
    cmdTimer.Enabled = False
    txtURL.Text = "http://mobile.alkar.net/sms.php?oper=050&number=" + txtNumber + "&text=" + txtMsg + "&sendform=%A0%CE%F2%EF%F0%E0%E2%E8%F2%FC%A0"
    Do
        inet.OpenURL (txtURL.Text)
        cx = cx + 1
        cmdLoop.Caption = cx
    Loop While (cx < txtCX.Text + 1)
    cmdTimer.Enabled = True
    cmdLoop.Caption = "Loop"
End Sub

Private Sub cmdSend_Click()
txtURL.Text = "http://mobile.alkar.net/sms.php?oper=050&number=" + txtNumber + "&text=" + txtMsg + "&sendform=%A0%CE%F2%EF%F0%E0%E2%E8%F2%FC%A0"
    inet.OpenURL (txtURL.Text)
End Sub

Private Sub cmdTimer_Click()
txtURL.Text = "http://mobile.alkar.net/sms.php?oper=050&number=" + txtNumber + "&text=" + txtMsg + "&sendform=%A0%CE%F2%EF%F0%E0%E2%E8%F2%FC%A0"
If tmrSend.Enabled = False Then
    cx = 0
    tmrSend.Interval = txtInterval.Text
    tmrSend.Enabled = True
    cmdLoop.Enabled = False
    Else
      tmrSend.Enabled = False
       cmdLoop.Enabled = True
       cmdTimer.Caption = "Timer"
       
End If
End Sub

Private Sub Form_Load()
txtURL.Text = "http://mobile.alkar.net/sms.php?oper=050&number=" + txtNumber + "&text=" + txtMsg + "&sendform=%A0%CE%F2%EF%F0%E0%E2%E8%F2%FC%A0"
End Sub


Private Sub Text2_Change()

End Sub

Private Sub tmrSend_Timer()

    inet.OpenURL (txtURL.Text)
    cx = cx + 1
    cmdTimer.Caption = cx
End Sub

