function xx( a , b )
  xx= a+b
end function

function yy( a , b )
  adsfdsaf
end function

function DeleteWebSite(sSiteName, sWebSiteIndex)
    On Error Resume Next
    Dim webSite, vRoot, vDir
    
    Set webSite = GetObject("IIS://LocalHost/W3svc/" & sWebSiteIndex)
    If Err.Number <> 0 Then
    	' "站点IIS://LocalhOST/W3SVC/" & sWebSiteIndex & "可能不存在。"
        DeleteWebSite = "1"
        Err.Clear
        Exit function
    End If
    
    Set vDir = GetObject("IIS://LocalHost/W3svc/" & sWebSiteIndex & "/Root/" & sSiteName)
    If Err.Number <> 0 Then
    	'"目录Del IIS://LocalHost/W3svc/" & sWebSiteIndex & "/Root/" & sSiteName & "不存在"
        DeleteWebSite = "3"
        Err.Clear
        Exit function
    End If
    
    Set vRoot = GetObject("IIS://LocalHost/W3svc/" & sWebSiteIndex & "/Root")
    If Err.Number <> 0 Then
      	'"站点IIS://LocalhOST/W3SVC/" & sWebSiteIndex & "可能不存在。"
        DeleteWebSite = "1"
        Err.Clear
        Exit function
    End If
        
    vRoot.Delete "IIsWebVirtualDir", sSiteName
    If Err.Number <> 0 Then
    	'"删除虚拟目录时出错"
        DeleteWebSite = "5"
        Err.Clear
        Exit function
    End If
    
    DeleteWebSite = "0"
End function

'=====================================================
' 判断站点 sSiteName 是否存在
'=====================================================
function ExitWebSite(sSiteName, sWebSiteIndex)
	On Error Resume Next
    Dim webSite, vDir
    
    Set webSite = GetObject("IIS://localhost/W3svc/" & sWebSiteIndex)
    If Err.Number <> 0 Then
		'站点IIS://LocalhOST/W3SVC/" & sWebSiteIndex & 可能不存在
		ExitWebSite = 0
		Exit function
    End If
    
    Set vDir = GetObject("IIS://Localhost/W3SVC/" & sWebSiteIndex & "/Root/" & sSiteName)
    If Err.Number <> 0 Then   'the site doesnt exist, so create it.
    	'虚拟目录不存在
        Err.Clear
        ExitWebSite = 0
        Exit function
    End If

    ExitWebSite = 1
End function
