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
    	' "վ��IIS://LocalhOST/W3SVC/" & sWebSiteIndex & "���ܲ����ڡ�"
        DeleteWebSite = "1"
        Err.Clear
        Exit function
    End If
    
    Set vDir = GetObject("IIS://LocalHost/W3svc/" & sWebSiteIndex & "/Root/" & sSiteName)
    If Err.Number <> 0 Then
    	'"Ŀ¼Del IIS://LocalHost/W3svc/" & sWebSiteIndex & "/Root/" & sSiteName & "������"
        DeleteWebSite = "3"
        Err.Clear
        Exit function
    End If
    
    Set vRoot = GetObject("IIS://LocalHost/W3svc/" & sWebSiteIndex & "/Root")
    If Err.Number <> 0 Then
      	'"վ��IIS://LocalhOST/W3SVC/" & sWebSiteIndex & "���ܲ����ڡ�"
        DeleteWebSite = "1"
        Err.Clear
        Exit function
    End If
        
    vRoot.Delete "IIsWebVirtualDir", sSiteName
    If Err.Number <> 0 Then
    	'"ɾ������Ŀ¼ʱ����"
        DeleteWebSite = "5"
        Err.Clear
        Exit function
    End If
    
    DeleteWebSite = "0"
End function

'=====================================================
' �ж�վ�� sSiteName �Ƿ����
'=====================================================
function ExitWebSite(sSiteName, sWebSiteIndex)
	On Error Resume Next
    Dim webSite, vDir
    
    Set webSite = GetObject("IIS://localhost/W3svc/" & sWebSiteIndex)
    If Err.Number <> 0 Then
		'վ��IIS://LocalhOST/W3SVC/" & sWebSiteIndex & ���ܲ�����
		ExitWebSite = 0
		Exit function
    End If
    
    Set vDir = GetObject("IIS://Localhost/W3SVC/" & sWebSiteIndex & "/Root/" & sSiteName)
    If Err.Number <> 0 Then   'the site doesnt exist, so create it.
    	'����Ŀ¼������
        Err.Clear
        ExitWebSite = 0
        Exit function
    End If

    ExitWebSite = 1
End function
