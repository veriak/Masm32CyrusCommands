;----------------------------------------------------------------------------------------
; Cyrus Commands Sample
;
; Copyright 2008 Veria Kalantari
; veria.kalantary@gmail.com
; https://github.com/veriak
;
;----------------------------------------------------------------------------------------

%include @Environ(OA32_PATH)\\Code\\Macros\\Model.inc
SysSetup OOP_PRIMER                     
                                                   
include Cyrus_Commands.inc                                 

.data	
	CyrusSendCommand		Pointer		NULL
	CyrusRecvCommand		Pointer		NULL	
	strValue				LPCSTR		NULL
	pParameters				PCyrusCommandParamArray	NULL

.code
start:
    SysInit   
    invoke CoInitialize, NULL    
    New CCyrusSendCommand
    mov CyrusSendCommand, eax
    New CCyrusRecvCommand
    mov CyrusRecvCommand, eax
    OCall CyrusSendCommand::CCyrusSendCommand.Init                    
    OCall CyrusSendCommand::CCyrusSendCommand.SetCommandText, StrM("Send SMS")       
    OCall CyrusSendCommand::CCyrusSendCommand.SetSource, StrM("node1")
    OCall CyrusSendCommand::CCyrusSendCommand.SetSourceType, StrM("Station Profile")
    OCall CyrusSendCommand::CCyrusSendCommand.SetTarget, StrM("SMS Server")
    OCall CyrusSendCommand::CCyrusSendCommand.SetTargetType, StrM("Service")
    OCall CyrusSendCommand::CCyrusSendCommand.SetReplyCommandText, StrM("Send SMS")
    OCall CyrusSendCommand::CCyrusSendCommand.SetReplyReceiver, StrM("SMS Server")
    OCall CyrusSendCommand::CCyrusSendCommand.SetReplyReceiverType, StrM("Service")        
    OCall CyrusSendCommand::CCyrusSendCommand.AddParameter, NULL, StrM("SMS"), NULL, NULL
    OCall CyrusSendCommand::CCyrusSendCommand.AddParameter, StrM("SMS"), StrM("Receiver_Mobile_Number"), NULL, StrM("989350000000")        
    OCall CyrusSendCommand::CCyrusSendCommand.AddParameter, StrM("SMS"), StrM("Message_Body"), NULL, StrM("test")         
    OCall CyrusSendCommand::CCyrusSendCommand.AddParameter, StrM("SMS"), StrM("Message_Type"), NULL, StrM("default")           
    OCall CyrusSendCommand::CCyrusSendCommand.AddParameter, StrM("SMS"), StrM("Message_Bin"), StrM("Bin"), StrM(6h,6h,"def",7h,8h,"ault")
    OCall CyrusSendCommand::CCyrusSendCommand.AddParameter, StrM("SMS"), StrM("Message_Bin2"), StrM("Bin"), StrM(6h,6h,"def",7h,8h,"ault2")
    OCall CyrusSendCommand::CCyrusSendCommand.AddParameter, StrM("SMS"), StrM("Message_Type2"), NULL, StrM("default2")        
;    OCall CyrusSendCommand::CCyrusSendCommand.AddReplyParameter, NULL, StrM("SMS"), NULL, NULL
;    OCall CyrusSendCommand::CCyrusSendCommand.AddReplyParameter, StrM("SMS"), StrM("Receiver_Mobile_Number"), NULL, StrM("989350000000")
;    OCall CyrusSendCommand::CCyrusSendCommand.AddReplyParameter, StrM("SMS"), StrM("Message_Body"), NULL, StrM("test2")
;    OCall CyrusSendCommand::CCyrusSendCommand.AddReplyParameter, StrM("SMS"), StrM("Message_Type"), NULL, StrM("Unicode")	
	OCall CyrusSendCommand::CCyrusSendCommand.GetAddInfo
	OCall CyrusSendCommand::CCyrusSendCommand.SetReplyAddInfo, eax					
	OCall CyrusSendCommand::CCyrusSendCommand.GetMsg
	pushad
    invoke MessageBox,0,eax,0,0
    popad    
    OCall CyrusRecvCommand::CCyrusRecvCommand.Init, eax   
    OCall CyrusSendCommand::CCyrusSendCommand.Clear 
    OCall CyrusRecvCommand::CCyrusRecvCommand.GetParameters
    mov pParameters, eax
    mov ebx, pParameters 
    add ebx, sizeof CyrusCommandParam
    add ebx, sizeof CyrusCommandParam
    add ebx, sizeof CyrusCommandParam
    add ebx, sizeof CyrusCommandParam        
    mov eax, (CyrusCommandParam ptr [ebx]).ParamValue         
    invoke MessageBox,0,eax,0,MB_OK        
    Destroy CyrusSendCommand
    Destroy CyrusRecvCommand    
    invoke CoUninitialize 	      
    SysDone                                         	
    invoke ExitProcess, 0                       
end start                                       
