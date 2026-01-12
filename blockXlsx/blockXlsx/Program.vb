Imports System.Net.Mime.MediaTypeNames
Imports Microsoft.Office.Interop.Excel

Public Class ExcelProtector
    Public Sub ProtectSheet(filePath As String, Optional password As String = "Zaqxcvbnm220033!!NAMI")
        Dim excelApp As New Microsoft.Office.Interop.Excel.Application
        Dim workbook As Workbook = Nothing

        Try
            Dim fullPath As String = System.IO.Path.GetFullPath(filePath)

            If Not System.IO.File.Exists(fullPath) Then
                Throw New Exception("Файл не найден: " & fullPath)
            End If


            excelApp.Visible = False
            workbook = excelApp.Workbooks.Open(fullPath)

            For Each sheet As Worksheet In workbook.Sheets
                sheet.Protect(Password:=password, UserInterfaceOnly:=True)
            Next

            workbook.Save()
            Console.WriteLine("SUCCESS: File protected - " & fullPath)
        Catch ex As Exception
            Throw New Exception("Error protecting Excel sheet: " & ex.Message)
        Finally
            If workbook IsNot Nothing Then
                workbook.Close()
            End If
            excelApp.Quit()
            ReleaseObject(workbook)
            ReleaseObject(excelApp)
        End Try
    End Sub

    Private Sub ReleaseObject(ByVal obj As Object)
        Try
            System.Runtime.InteropServices.Marshal.ReleaseComObject(obj)
            obj = Nothing
        Catch
            obj = Nothing
        Finally
            GC.Collect()
        End Try
    End Sub
End Class


Module Program
    Sub Main(args As String())

        If args.Length = 0 Then
            Console.WriteLine("ERROR: Path to Excel file not provided")
            Console.WriteLine("Usage: ExcelProtector.exe <path_to_file>")
            Environment.ExitCode = 1
            Return
        End If

        Dim filePath As String = args(0)

        Try
            Dim protector As New ExcelProtector()
            protector.ProtectSheet(filePath)
            Console.WriteLine("OK")
            Environment.ExitCode = 0
        Catch ex As Exception
            Console.WriteLine("ERROR: " & ex.Message)
            Environment.ExitCode = 1
        End Try
    End Sub
End Module
