

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

##Successful cases
- Nextinstaller
   Nextinstaller is a powerful installation package making tool, which internally uses the atscript script engine as the driver of the installation wizard
   website: [http://ni.wanzqh.com](http://ni.wanzqh.com)
- Expressive, intuitive [database ORM](https://laravel.com/docs/eloquent).
- [Powerful dependency injection container](https://laravel.com/docs/container).
- Multiple back-ends for [session](https://laravel.com/docs/session) and [cache](https://laravel.com/docs/cache) storage.
- Expressive, intuitive [database ORM](https://laravel.com/docs/eloquent).
- Database agnostic [schema migrations](https://laravel.com/docs/migrations).
- [Robust background job processing](https://laravel.com/docs/queues).
- [Real-time event broadcasting](https://laravel.com/docs/broadcasting).


- **[Vehikl](https://vehikl.com/)**
- **[Tighten Co.](https://tighten.co)**
- **[Kirschbaum Development Group](https://kirschbaumdevelopment.com)**
- **[64 Robots](https://64robots.com)**
- **[Cubet Techno Labs](https://cubettech.com)**
- **[Cyber-Duck](https://cyber-duck.co.uk)**
- **[Many](https://www.many.co.uk)**
- **[Webdock, Fast VPS Hosting](https://www.webdock.io/en)**
- **[DevSquad](https://devsquad.com)**
- **[Curotec](https://www.curotec.com/services/technologies/laravel/)**
- **[OP.GG](https://op.gg)**
- **[CMS Max](https://www.cmsmax.com/)**
- **[WebReinvent](https://webreinvent.com/?utm_source=laravel&utm_medium=github&utm_campaign=patreon-sponsors)**
- **[Lendio](https://lendio.com)**
- **[Romega Software](https://romegasoftware.com)**

## Contributing

Thank you for considering contributing to the Laravel framework! The contribution guide can be found in the [Laravel documentation](https://laravel.com/docs/contributions).

## Code of Conduct

In order to ensure that the Laravel community is welcoming to all, please review and abide by the [Code of Conduct](https://laravel.com/docs/contributions#code-of-conduct).

## Security Vulnerabilities

If you discover a security vulnerability within Laravel, please send an e-mail to Taylor Otwell via [taylor@laravel.com](mailto:taylor@laravel.com). All security vulnerabilities will be promptly addressed.

## License

The Laravel framework is open-sourced software licensed under the [MIT license](https://opensource.org/licenses/MIT).
