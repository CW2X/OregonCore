Imports System.IO

Public Class Form1
    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click

        'get *.h file lists
        Dim di As New DirectoryInfo("F:\WOWServer\Source\OregonCore-Rebase\src\game")

        Dim dil As DirectoryInfo() = di.GetDirectories

        Dim dictFI As Dictionary(Of String, String) = New Dictionary(Of String, String)

        For Each d In dil
            Dim fil As FileInfo() = d.GetFiles
            For Each f In fil
                If f.Extension = ".h" Then
                    If Not dictFI.Keys.Contains(f.Name) Then
                        dictFI.Add("#include """ & f.Name & """", "#include """ & f.Directory.Name & "/" & f.Name & """")
                    End If
                End If
            Next
        Next

        'rewrite header inside
        'For Each d In dil
        '    Dim fil As FileInfo() = d.GetFiles
        '    For Each f In fil
        '        rewriteheader(f.FullName, dictFI)
        '    Next
        'Next
        GetAllFiles("F:\WOWServer\Source\OregonCore-Rebase\src\game", dictFI)
        MsgBox("done")
    End Sub

    Sub rewriteheader(ByVal path As String, ByVal dict As Dictionary(Of String, String))
        Dim finalContent As String = ""
        'read and replace file
        Dim line As String
        Dim sr As StreamReader = New StreamReader(path, System.Text.Encoding.UTF8)
        Do While sr.Peek() > 0
            line = sr.ReadLine()
            If dict.ContainsKey(line.Trim()) Then
                finalContent &= dict(line.Trim()) & vbCrLf
            Else
                finalContent &= line & vbCrLf
            End If
        Loop
        sr.Close()
        sr = Nothing

        'write
        Dim sw As StreamWriter = New StreamWriter(path, False, System.Text.Encoding.UTF8) 'true is append method

        sw.WriteLine(finalContent)
        sw.Flush()

        sw.Close()
        sw = Nothing
    End Sub

    Private Sub subTXTWrite()
        Dim strFilePath As String = "D:\test.txt"
        Dim temp
        Dim sw As StreamWriter = New StreamWriter(strFilePath, True, System.Text.Encoding.UTF8) 'true is append method
        For i = 0 To 10
            temp = i.ToString
            sw.WriteLine(temp)
            sw.Flush()
        Next
        sw.Close()
        sw = Nothing
    End Sub

    Private Sub subTXTRead()
        Dim line As String
        Dim sr As StreamReader = New StreamReader("D:\test.txt", System.Text.Encoding.UTF8)
        Do While sr.Peek() > 0
            line = sr.ReadLine()
        Loop
        sr.Close()
        sr = Nothing
    End Sub


    Private Sub GetAllFiles(ByVal strDirect As String, ByVal dict As Dictionary(Of String, String))  '搜索所有目录下的文件
        If Not (strDirect Is Nothing) Then
            Dim mFileInfo As System.IO.FileInfo
            Dim mDir As System.IO.DirectoryInfo
            Dim mDirInfo As New System.IO.DirectoryInfo(strDirect)
            Try
                For Each mFileInfo In mDirInfo.GetFiles("*.h")
                    'Debug.Print(mFileInfo.FullName)
                    rewriteheader(mFileInfo.FullName, dict)
                Next

                For Each mFileInfo In mDirInfo.GetFiles("*.cpp")
                    rewriteheader(mFileInfo.FullName, dict)
                Next

                For Each mDir In mDirInfo.GetDirectories
                    'Debug.Print("******目录回调*******")
                    GetAllFiles(mDir.FullName, dict)
                Next
            Catch ex As System.IO.DirectoryNotFoundException
                Debug.Print("目录没找到：" + ex.Message)
            End Try
        End If
    End Sub

End Class
