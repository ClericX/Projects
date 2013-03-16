Module Module1

    Sub Main()
        Dim ip As String = "192.168.0.1"
        Dim ipSplit As String() = Split(ip, ".")
        Dim ipBinary As String = ""
        Dim ipHex As String = ""

        ' conversion to binary
        For i As Integer = 0 To 3
            Dim ipPart As String = Convert.ToString(Convert.ToInt32(ipSplit(i)), 2)
            If ipPart.Length < 8 Then
                For x As Integer = 0 To (7 - ipPart.Length)
                    ipPart = "0" + ipPart
                Next
            End If
            ipBinary += ipPart
            If i < 3 Then
                ipBinary += "."
            End If
        Next

        ' conversion to hex
        For i As Integer = 0 To 3
            Dim ipPart As String = Convert.ToString(Convert.ToInt32(ipSplit(i)), 16)
            If ipPart.Length < 2 Then
                ipPart = "0" + ipPart
            End If
            ipHex += ipPart
            If i < 3 Then
                ipHex += "."
            End If
        Next

        Console.WriteLine(ip & " | BIN: " & ipBinary & " | HEX: " & ipHex)
        Console.ReadKey()
    End Sub

End Module
