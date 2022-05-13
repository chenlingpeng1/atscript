

## Design and implementation of commercial lightweight script engine (atscript)

Design purpose: providing script support for applications through embedded script engine is an effective way to realize the customization and scalability of applications, but the existing scripts are either inconvenient in syntax or The engine is too large, which reduces the efficiency of the application. It is either inconvenient to cross platform, or it is not easy to expand and modify. In order to solve this problem, a script with simple syntax and easy to learn is designed Language atscript, which realizes the lightweight script engine of atscript. The engine supports scripts to seamlessly use classes and functions implemented by applications, and has good execution performance andScalability, the most important is small, and convenient to run in various systems or mobile terminals.

Atscript script language, with its powerful extension ability and flexible syntax structure class JavaScript, makes it possible to provide script support for applications, so as to develop mixed languages and realize extensibility and flexibility customizable and effective solutions. In consideration of stability and development time constraints, developers tend to embed the existing script engine to provide script support for applications,For example, the embedded Python engine provides Python script support for the application, or uses the ActiveX scripting technology provided by Microsoft to provide corresponding script support for the embedded VBScript engine or JavaScript engine of the application.However, the flexibility of this method is poor, and the application must accept the volume and performance requirements of the existing script engine, which is for applications running under low hardware conditions or small applications that only require simple rule calculation,This method has no advantage in efficiency. Moreover, unified deployment is not convenient. Therefore, we need a lightweight script engine, which can be easily embedded into the code. Aiming at the above problems, this paper designs a script by ourselves.Based on atscript language, a lightweight script engine of atscript is developed and implemented. The script engine is embedded in C + +, Java, c# And other applications. The syntax of atscript language is simple and inherited from non professional programmers JavaScript language, which has a high reputation in, and draws lessons from some functions of c# language, so that users can focus on problem solving rather than grammar learning.


## Specific use of atscript
### 1 Embed atscript into code

	CAtsEngine Eng;
	Eng.LoadScript( "function void Main() \r\n{\r\n MessageBox( \"hello\" ); \r\n}" );
	if( !Eng.RunSegment( "Main") )	
	{
		strPrompt.Format("Script is error, ErrorCode=%d,Line = %d,ErrorWord=%s" , Eng.m_nLastErrorCode , Eng.m_nErrorLine , Eng.m_strErrorWord );
		AfxMessageBox( strPrompt );
	}

### 2 Extend users' own objects to realize function integration

	CAtsEngine		Eng;
	CMyAtsObj * pNewObj = new CMyAtsObj;
	pNewObj->m_pEngine = &Eng;
	Eng.AddObjIns( "MyObj" , pNewObj );
	Eng.LoadScript( script );
	if( !Eng.RunSegment( "Main") )	
	{
		strPrompt.Format("Script is error, ErrorCode=%d,Line = %d,ErrorWord=%s" , Eng.m_nLastErrorCode , Eng.m_nErrorLine , Eng.m_strErrorWord );
		AfxMessageBox( strPrompt );
	}

	Among them, cmyatsobj is an extension class written by users themselves. Through their own extension, function integration can be easily realized. The following is the reference script:

	function void Main()
       { 
           var string a;
           a = myobj.getname();
           MessageBox(a);
       }

### 3 Extension constants
	CAtsEngine		Eng;
	CMyAtsObj * pNewObj = new CMyAtsObj;
	pNewObj->m_pEngine = &Eng;
	Eng.SetConstVar("SS_INSTALL" , "1", ATSVAR_TYPE_INT );
	Eng.LoadScript( script );
	if( !Eng.RunSegment( "Main") )	
	{
		strPrompt.Format("Script is error, ErrorCode=%d,Line = %d,ErrorWord=%s" , Eng.m_nLastErrorCode , Eng.m_nErrorLine , Eng.m_strErrorWord );
		AfxMessageBox( strPrompt );
	}
     
       Where SS_INSTALL is the user's extension constant. The following is the reference script:
       function void Main()
       { 
           var int a;
           a = SS_INSTALL;
           MessageBox(a);
       }

### 4 Extend your own class
	CAtsEngine		Eng;

	CMyAtsObj * pNewObj = new CMyAtsObj;
	pNewObj->m_pEngine = &Eng;
	Eng.AddClassTemplate( new CMyClass );
	Eng.LoadScript( script );
	if( !Eng.RunSegment( "Main") )	
	{
		strPrompt.Format("Script is error, ErrorCode=%d,Line = %d,ErrorWord=%s" , Eng.m_nLastErrorCode , Eng.m_nErrorLine , Eng.m_strErrorWord );
		AfxMessageBox( strPrompt );
	}

     Where CMyClass is the user's extended class. The following is the reference script:
     function void Main() 
     {
  	var CMyClass MyClass;
  	MyClass.SetData( "abc" );
  	MessageBox( MyClass.GetData() );
      }

## Support

- mail:[905309882@qq.com](mailto:905309882@qq.com).

## Successful cases
- Nextinstaller
   Nextinstaller is a powerful installation package making tool, which internally uses the atscript script engine as the driver of the installation wizard.
   website: [http://ni.wanzqh.com](http://ni.wanzqh.com)
- firspider
   A two-dimensional game engine running on the mobile phone is used for the production of small games, which is conducive to the learning and Enlightenment of game development. Atscript script is used internally to drive the game.
   website: [http://firespider.wanzqh.com](http://firespider.wanzqh.com)

- quickspider
   Use atscript as a page capture engine

- quickdove
   An enterprise instant messaging tool, which internally uses atscript as the script developed by the plug-in, and can be used to integrate with the third-party user authentication system.
   website: http://www.quickdove.com](http://www.quickdove.com)

- **[Vehikl](https://vehikl.com/)**


## License
The code can be used for learning and research or for the development of shared software. If it is to be used in commercial applications, it needs the authorization of the author [905309882@qq.com](mailto:905309882@qq.com).
