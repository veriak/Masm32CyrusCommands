;----------------------------------------------------------------------------------------
; Cyrus Commands Library
;
; Copyright 2008 Veria Kalantari
; veria.kalantary@gmail.com
; https://github.com/veriak
;
;----------------------------------------------------------------------------------------

include XMLDocument.inc

CyrusCommandParam struct
  ParamName		LPCSTR	?
  ParamType		LPCSTR	?
  ParamValue	DWORD	?
CyrusCommandParam ends
PCyrusCommandParamArray	typedef DWORD

ReplacedByte struct
    iIndex		DWORD	?
    chOriginal	BYTE	?
ReplacedByte ends
PReplacedBytes			typedef	DWORD

CyrusRoute struct
    Command				LPCSTR	?
    _Target				LPCSTR	?
    TargetType			LPCSTR	?
    Source				LPCSTR	?
    SourceType			LPCSTR	?
    ReplyReceiver		LPCSTR	?
    ReplyReceiverType	LPCSTR	?
    CommandAlias		LPCSTR	?
    ReplyCommandText	LPCSTR	?
    TRCTParamCount		DWORD	?
    TRCT 				PCyrusCommandParamArray	?
    ReplyTRCTParamCount	DWORD	?
    ReplyTRCT			PCyrusCommandParamArray	?
CyrusRoute ends
PCyrusRoutes 			typedef	DWORD

.const
  szCyrusCommandXMLHeader		db '<Cyrus_Commands Size="45"></Cyrus_Commands>', 0
  szRouteTableXMLHeader 		db '<Cyrus_Commands></Cyrus_Commands>', 0
  szNestedParam					db "Nested Param", 0
  szType						db "Type", 0
  szCommandText					db "Command_Text", 0
  szAddInfo						db "Add_Info", 0
  szReplyAddInfo				db "Reply_Add_Info", 0
  szBinaryStream				db "Binary_Stream", 0
  szSize						db "Size", 0
  szReplacedBytes				db "Replaced_Bytes", 0
  szCommand						db "Command", 0
  szBin							db "Bin", 0
  szTarget						db "Target", 0
  szSource						db "Source", 0
  szReplyReceiver				db "Reply_Receiver", 0
  szReplyCommandText			db "Reply_Command_Text", 0
  szCyrusCommands				db "Cyrus_Commands", 0
  szCyrusCommandsFlag			db "<Cyrus_Commands Size=", 0

;	//	CCyrus Object Definition
Object CCyrus, ,Primer 
	RedefineMethod    	Done
	RedefineMethod		Init	
	StaticMethod		Clear
	Private	
	StaticMethod		FinalExceptionHandler, 	DWORD	
	StaticMethod 		AddNode, 				LPCSTR, LPCSTR
	StaticMethod		AddBinaryNode,			PXMLNode, LPCSTR
    StaticMethod 		SetNodeAttribute,		LPCSTR, LPCSTR, LPCSTR
    StaticMethod		CopyNestedNode, 		PXMLNode, PXMLNode, PXMLNode          
       	
  	Embed				RootNode, 				CXMLNode    
    Embed				XMLDocument, 			CXMLDocument		
ObjectEnd

;	//	CCyrusCustomCommand Object Definition
Object CCyrusCustomCommand, , CCyrus
	RedefineMethod    	Done      
    RedefineMethod		Init    
    RedefineMethod		Clear    
    StaticMethod		FreeCommandAllocatedMem
    StaticMethod		GetMsg
    StaticMethod 		GetSource
    StaticMethod 		GetSourceType
    StaticMethod 		GetTarget
    StaticMethod 		GetTargetType
    StaticMethod		GetReplyReceiver
    StaticMethod 		GetReplyReceiverType
    StaticMethod		GetCommandText
    StaticMethod		GetReplyCommandText
    StaticMethod		GetAddInfo
    StaticMethod 		GetReplyAddInfo    
	Private	
    StaticMethod		AddParam,				LPCSTR, LPCSTR, LPCSTR, LPCSTR      
    StaticMethod		AddReplyParam,			LPCSTR, LPCSTR, LPCSTR, LPCSTR				
	
	Embed				LastNode,				CXMLNode
    Embed				AddInfoNode,			CXMLNode
    Embed				ReplyAddInfoNode,		CXMLNode
    DefineVariable		FCommandText,			LPCSTR,		NULL
    DefineVariable		FReplyCommandText,		LPCSTR,		NULL                
    DefineVariable		FSource,				LPCSTR,		NULL
    DefineVariable		FSourceType,			LPCSTR,		NULL
    DefineVariable		FTarget,				LPCSTR,		NULL
    DefineVariable		FTargetType,			LPCSTR,		NULL
    DefineVariable		FReplyReceiver,			LPCSTR,		NULL
    DefineVariable		FReplyReceiverType,		LPCSTR,		NULL
    DefineVariable		FAddInfo,				LPCSTR,		NULL    
    DefineVariable		FReplyAddInfo,			LPCSTR,		NULL
ObjectEnd

;	//	CCyrusSendCommand Object Definition
Object CCyrusSendCommand, , CCyrusCustomCommand
	RedefineMethod    	Done 
	RedefineMethod		Init    
    RedefineMethod		Clear    	
    StaticMethod		SetCommandText,			LPCSTR			
    StaticMethod		SetTarget,				LPCSTR
    StaticMethod		SetTargetType,			LPCSTR
    StaticMethod		SetSource,				LPCSTR
    StaticMethod		SetSourceType,			LPCSTR
    StaticMethod		SetReplyReceiver,		LPCSTR    
    StaticMethod		SetReplyReceiverType,	LPCSTR
    StaticMethod		SetReplyCommandText,	LPCSTR
    StaticMethod		SetAddInfo,				LPCSTR
    StaticMethod		SetReplyAddInfo,		LPCSTR
    StaticMethod		SetSize,				DWORD 
    StaticMethod		GetSize   
    StaticMethod		CalculateSize
	StaticMethod		AddParameter,			LPCSTR, LPCSTR, LPCSTR, LPCSTR
    StaticMethod		AddReplyParameter,		LPCSTR, LPCSTR, LPCSTR, LPCSTR
ObjectEnd

;	//	CCyrusRecvCommand Object Definition
Object CCyrusRecvCommand, , CCyrusCustomCommand 
	RedefineMethod    	Done
	RedefineMethod		Init,					LPCSTR				    
    RedefineMethod		Clear  
    StaticMethod 		SetMsg,					LPCSTR
    StaticMethod 		GetParametersCount
    StaticMethod 		GetReplyParametersCount        
    StaticMethod 		GetParameters
    StaticMethod		GetReplyParameters
    StaticMethod		GetSize
    StaticMethod 		IsCyrusCommand
    StaticMethod		HasCyrusTags        
	Private    
    StaticMethod		ParseMsg,				LPCSTR
    StaticMethod		ParseNestedParam,		PXMLNode, DWORD, PDWORD
        
    DefineVariable		FParameters,			PCyrusCommandParamArray,	NULL
    DefineVariable		FReplyParameters,		PCyrusCommandParamArray,	NULL
    DefineVariable		FSize,					DWORD,		0
    DefineVariable		FParametersCount,		DWORD,		0
    DefineVariable		FReplyParametersCount,	DWORD,		0
    DefineVariable		FIsCyrusCommand,		BOOLEAN,	?
    DefineVariable		FHasCyrusTags,			BOOLEAN,	?			
ObjectEnd

;	//	CCyrus Object Implementation
Method CCyrus.FinalExceptionHandler, uses esi edi ebx, lpException:DWORD
;ClearUp	
	mov eax, 0
MethodEnd

Method CCyrus.Done, uses esi 	
	SetObject esi		
	.if [esi].RootNode.pNode		
		OCall [esi].RootNode::CXMLNode.Done
	.endif		
	OCall [esi].XMLDocument::CXMLDocument.Done	
	ACall esi.Done
MethodEnd

Method CCyrus.Init, uses esi					
	SetObject esi	
	invoke SetUnhandledExceptionFilter, $MethodAddr(esi::CCyrus.FinalExceptionHandler)	 
	OCall [esi].XMLDocument::CXMLDocument.Init
MethodEnd    

Method CCyrus.Clear, uses esi
	SetObject esi	
	.if [esi].RootNode.pNode				
		OCall [esi].RootNode::CXMLNode.Done
	.endif
	OCall [esi].XMLDocument::CXMLDocument.Clear			
MethodEnd

Method CCyrus.AddNode, uses esi, NodeName:DWORD, Value:DWORD
	LOCAL NewNode: CXMLNode
	LOCAL i: DWORD
  	LOCAL tempXMLDoc: CXMLDocument  	   	   	
  	  	 		
	assume fs:nothing
   	push offset @@OnExcept
   	push fs:[0]
   	mov fs:[0], esp

	SetObject esi	
	LNew NewNode, CXMLNode
	OCall NewNode::CXMLNode.Init		
	OCall [esi].RootNode::CXMLNode.HasChildNodes	   
    .if eax == FALSE    	    	
    	OCall [esi].RootNode::CXMLNode.AddChild, NodeName    	    	     	
     	OCall NewNode::CXMLNode.SetNode, eax
    .else   
    	OCall [esi].RootNode::CXMLNode.GetChildNode, 0 
      	OCall NewNode::CXMLNode.SetNode, eax
      	OCall [esi].RootNode::CXMLNode.GetChildNodesCount
      	mov i, eax      	
      	.while i
      		OCall NewNode::CXMLNode.GetName
      		push eax
      		invoke lstrcmp, eax, NodeName
      		.if eax == 0 
      			pop eax
      			invoke GlobalFree, eax
      			.break
      		.endif
      		pop eax
      		invoke GlobalFree, eax
        	dec i
        	.if i
          		OCall NewNode::CXMLNode.NextSibling
          		push eax
          		OCall NewNode::CXMLNode.Done
          		pop eax
          		OCall NewNode::CXMLNode.SetNode, eax
          	.endif	
      	.endw
      	.if i == 0
      		OCall NewNode::CXMLNode.Done
        	OCall [esi].RootNode::CXMLNode.AddChild, NodeName
     		OCall NewNode::CXMLNode.SetNode, eax
     	.endif	
    .endif	        	
	invoke lstrcmp, NodeName, addr szAddInfo  
    .if eax == 0    	
    	LNew tempXMLDoc, CXMLDocument    
    	OCall tempXMLDoc::CXMLDocument.Init    	    	      	
      	OCall tempXMLDoc::CXMLDocument.LoadFromXML, Value     	
      	OCall esi.CopyNestedNode, addr tempXMLDoc.DocumentElement, addr NewNode, addr [esi].RootNode
      	OCall tempXMLDoc::CXMLDocument.Done    	      
      	jmp @F    
    .endif         
    invoke lstrcmp, NodeName, addr szReplyAddInfo
    .if eax == 0
    	LNew tempXMLDoc, CXMLDocument    
    	OCall tempXMLDoc::CXMLDocument.Init    	    	
      	OCall tempXMLDoc::CXMLDocument.LoadFromXML, Value      	          	  	      	      	 	 
      	OCall esi.CopyNestedNode, addr tempXMLDoc.DocumentElement, addr NewNode, addr [esi].RootNode
		OCall tempXMLDoc::CXMLDocument.Done		        	 	  
      	jmp @F    
    .endif           
    OCall NewNode::CXMLNode.SetText, Value 
    OCall [esi].RootNode::CXMLNode.AppendToChildList, NewNode.pNode            
@@: 			
	mov eax, NewNode.pNode 		 
	jmp @@Exit
@@OnExcept:	
	mov eax, -1		
@@SafeExit:	
@@Exit:      
   	pop fs:[0]
   	add esp, 4  		
MethodEnd

Method CCyrus.AddBinaryNode, uses esi ebx edx, BinNode:PXMLNode, Value:DWORD
	LOCAL NewNode: CXMLNode			
	LOCAL TempNode: CXMLNode
	LOCAL i: DWORD 
  	LOCAL iLenBuff: DWORD   	  	  	
  	LOCAL pBuff: DWORD
  	LOCAL temp1[16]: BYTE
  	LOCAL temp2[16]: BYTE 
	
	assume fs:nothing
   	push offset @@OnExcept
   	push fs:[0]
   	mov fs:[0], esp
   	
	SetObject esi		
	LNew NewNode, CXMLNode
	OCall NewNode::CXMLNode.Init
	LNew TempNode, CXMLNode
	OCall TempNode::CXMLNode.Init
	
    OCall [BinNode]::CXMLNode.AddChild, addr szReplacedBytes        	
    OCall NewNode::CXMLNode.SetNode, eax   
    OCall [esi].RootNode::CXMLNode.AppendToChildList, NewNode.pNode
    invoke lstrlen, Value    
    mov iLenBuff, eax    
    inc eax
    invoke GlobalAlloc, GPTR, eax
    mov pBuff, eax 
    push eax 
    invoke memcpy, Value, pBuff, iLenBuff          		        		          		
    mov ebx, 0 
    .while ebx < iLenBuff
    	xor edx, edx    
    	mov eax, pBuff	    	    	    	
        mov dl, byte ptr [eax]         
        .if (edx >= 00h && edx <= 09h) || (edx == 0Bh) || (edx == 0Ch) || \
        	(edx >= 0Eh && edx <= 1Fh)        
        	push edx        		
        	invoke lstrcpy, addr temp1, StrM("b")
        	invoke dwtoa, ebx, addr temp2        	  
           	invoke lstrcat, addr temp1, addr temp2           			
           	OCall NewNode::CXMLNode.AddChild, addr temp1           	
           	OCall TempNode::CXMLNode.SetNode, eax
           	OCall [esi].RootNode::CXMLNode.AppendToChildList, TempNode.pNode
           	pop edx	   
           	invoke dwtoa, edx, addr temp1              
           	OCall TempNode::CXMLNode.SetText, addr temp1
           	mov eax, pBuff         	     	         		              			              			
           	mov byte ptr [eax], 2Ah
        .endif
        inc ebx
        inc pBuff
    .endw    
    OCall [BinNode]::CXMLNode.AddChild,addr szBinaryStream    
    OCall NewNode::CXMLNode.SetNode, eax
    invoke dwtoa, iLenBuff, addr temp1
    OCall NewNode::CXMLNode.SetAttribute, addr szSize, addr temp1     
    pop ebx    		          		
    OCall NewNode::CXMLNode.SetText, ebx  
    invoke GlobalFree, ebx  
    OCall [esi].RootNode::CXMLNode.AppendToChildList, NewNode.pNode
	mov eax, NewNode.pNode      	    
    jmp @@Exit
@@OnExcept:
	mov eax, -1		
@@SafeExit:	
@@Exit:      
   	pop fs:[0]
   	add esp, 4  			
MethodEnd

Method CCyrus.SetNodeAttribute, uses esi, NodeName:DWORD, Attribute:DWORD, Value:DWORD
	LOCAL NewNode: CXMLNode
	LOCAL i: DWORD 	
	
	assume fs:nothing
   	push offset @@OnExcept
   	push fs:[0]
   	mov fs:[0], esp
   	
	SetObject esi
	LNew NewNode, CXMLNode
	OCall NewNode::CXMLNode.Init	
		 
    OCall [esi].RootNode::CXMLNode.HasChildNodes
    .if eax == TRUE      	
    	OCall [esi].RootNode::CXMLNode.GetChildNode, 0  	   	     	    	
    	OCall NewNode::CXMLNode.SetNode, eax  	
    	OCall [esi].RootNode::CXMLNode.GetChildNodesCount
      	mov i, eax      	      	    	      	
      	.while i > 0
      		OCall NewNode::CXMLNode.GetName
      		push eax      		      		
      		invoke lstrcmp, eax, NodeName
      		.if eax == 0 
      			pop eax
      			invoke GlobalFree, eax        			   			 
      			.break
      		.endif  
      		pop eax
      		invoke GlobalFree, eax    		
        	dec i
        	.if i        		
          		OCall NewNode::CXMLNode.NextSibling
          		push eax
          		OCall NewNode::CXMLNode.Done
          		pop eax
          		OCall NewNode::CXMLNode.SetNode, eax          	
          	.endif	
      	.endw      	
      	.if i       		
      		OCall NewNode::CXMLNode.SetAttribute, Attribute, Value      		
      	.endif	     	
      	OCall NewNode::CXMLNode.Done
    .endif        
 	jmp @@Exit
@@OnExcept:		
	mov eax, -1		
@@SafeExit:	
@@Exit:      
   	pop fs:[0]
   	add esp, 4  	
MethodEnd

Method CCyrus.CopyNestedNode, uses esi edi ebx, SrcNode:PXMLNode, DestNode:PXMLNode, RootNode:PXMLNode
	LOCAL TempSrcNode: CXMLNode
	LOCAL TempDestNode: CXMLNode
  	LOCAL i: DWORD
  	LOCAL iMax: DWORD
  	
	SetObject esi
	LNew TempSrcNode, CXMLNode
	OCall TempSrcNode::CXMLNode.Init
	LNew TempDestNode, CXMLNode
	OCall TempDestNode::CXMLNode.Init				
	OCall SrcNode::CXMLNode.GetChildNode, 0	
	OCall TempSrcNode::CXMLNode.SetNode, eax  	    	  	  		  	  	
  	OCall SrcNode::CXMLNode.GetChildNodesCount
  	mov iMax, eax
	mov i, 1	
	.while TRUE					
		OCall TempSrcNode::CXMLNode.GetName		
		push eax					
		OCall DestNode::CXMLNode.AddChild, eax			
		OCall TempDestNode::CXMLNode.SetNode, eax				
		OCall RootNode::CXMLNode.AppendToChildList, eax
  		pop eax
  		invoke GlobalFree, eax
  		OCall TempSrcNode::CXMLNode.HasAttribute, addr szType
    	.if eax
    		OCall TempSrcNode::CXMLNode.GetAttribute, addr szType    		
    		push eax
		    OCall TempDestNode::CXMLNode.SetAttribute, addr szType, eax
		    pop eax
		    invoke GlobalFree, eax		    			
		.endif			
		mov edi, $OCall(TempSrcNode::CXMLNode.IsTextElement)							
		OCall TempSrcNode::CXMLNode.GetChildNodesCount						
      	.if eax == 0
        	OCall TempDestNode::CXMLNode.SetText, NULL       		        	
      	.elseif (eax == 1) && (edi == TRUE)       		      		
        	OCall TempSrcNode::CXMLNode.GetText
        	push eax
        	OCall TempDestNode::CXMLNode.SetText, eax
        	pop eax
        	invoke GlobalFree, eax        		        	
      	.else	   	      		
        	OCall esi.CopyNestedNode, addr TempSrcNode, addr TempDestNode, RootNode        	 
    	.endif 
    	ACall TempDestNode::CXMLNode.Done    	    	
    	inc i
    	mov ebx, iMax
    	.if i <= ebx     			  	  	
    		OCall TempSrcNode::CXMLNode.NextSibling 
    		push eax
    		OCall TempSrcNode::CXMLNode.Done
    		pop eax   		
    		OCall TempSrcNode::CXMLNode.SetNode, eax   					
	    .else	  	    	 		    	 
	    	OCall TempSrcNode::CXMLNode.Done
	    	.break
	    .endif		
  	.endw   	 	  
MethodEnd

;	//	CCyrusCustomCommand Object Implementation
Method CCyrusCustomCommand.Init, uses esi
	SetObject esi
	ACall esi.Init
MethodEnd

Method CCyrusCustomCommand.Done, uses esi
	SetObject esi
	OCall esi.FreeCommandAllocatedMem						
	ACall esi.Done		
MethodEnd

Method CCyrusCustomCommand.Clear, uses esi
	SetObject esi			
	OCall esi.FreeCommandAllocatedMem	
	ACall esi.Clear			  		  	  			  	
MethodEnd
	
Method CCyrusCustomCommand.FreeCommandAllocatedMem, uses esi
	SetObject esi
	.if [esi].FSource
		invoke GlobalFree, [esi].FSource
		mov [esi].FSource, NULL
	.endif
	.if [esi].FSourceType	
  		invoke GlobalFree, [esi].FSourceType
  		mov [esi].FSourceType, NULL
  	.endif
  	.if [esi].FTarget	
  		invoke GlobalFree, [esi].FTarget
  		mov [esi].FTarget, NULL
  	.endif
  	.if [esi].FTargetType	
  		invoke GlobalFree, [esi].FTargetType
  		mov [esi].FTargetType, NULL
  	.endif
  	.if [esi].FReplyReceiver	
  		invoke GlobalFree, [esi].FReplyReceiver
  		mov [esi].FReplyReceiver, NULL
  	.endif
  	.if	[esi].FReplyReceiverType
  		invoke GlobalFree, [esi].FReplyReceiverType
  		mov [esi].FReplyReceiverType, NULL
  	.endif
  	.if [esi].FCommandText	
  		invoke GlobalFree, [esi].FCommandText
  		mov [esi].FCommandText, NULL
  	.endif
  	.if [esi].FReplyCommandText	
  		invoke GlobalFree, [esi].FReplyCommandText
  		mov [esi].FReplyCommandText, NULL
  	.endif
  	.if [esi].FAddInfo	  
  		invoke GlobalFree, [esi].FAddInfo
  		mov [esi].FAddInfo, NULL
  	.endif
  	.if [esi].FReplyAddInfo	
  		invoke GlobalFree, [esi].FReplyAddInfo
  		mov [esi].FReplyAddInfo, NULL
  	.endif	
  	.if [esi].AddInfoNode.pNode  	
		OCall [esi].AddInfoNode::CXMLNode.Done
	.endif
	.if [esi].ReplyAddInfoNode.pNode
		OCall [esi].ReplyAddInfoNode::CXMLNode.Done
	.endif			
MethodEnd
	
Method CCyrusCustomCommand.GetMsg, uses esi   
	SetObject esi
	OCall [esi].XMLDocument::CXMLDocument.GetXML
MethodEnd

Method CCyrusCustomCommand.GetCommandText, uses esi   
	SetObject esi	
	mov eax, [esi].FCommandText
MethodEnd

Method CCyrusCustomCommand.GetReplyCommandText, uses esi   
	SetObject esi
	mov eax, [esi].FReplyCommandText	
MethodEnd

Method CCyrusCustomCommand.GetTarget, uses esi   
	SetObject esi	
	mov eax, [esi].FTarget
MethodEnd

Method CCyrusCustomCommand.GetTargetType, uses esi   
	SetObject esi	
	mov eax, [esi].FTargetType
MethodEnd

Method CCyrusCustomCommand.GetSource, uses esi   
	SetObject esi	
	mov eax, [esi].FSource
MethodEnd

Method CCyrusCustomCommand.GetSourceType, uses esi   
	SetObject esi
	mov eax, [esi].FSourceType	
MethodEnd

Method CCyrusCustomCommand.GetReplyReceiver, uses esi   
	SetObject esi	
	mov eax, [esi].FReplyReceiver
MethodEnd

Method CCyrusCustomCommand.GetReplyReceiverType, uses esi   
	SetObject esi	
	mov eax, [esi].FReplyReceiverType
MethodEnd

Method CCyrusCustomCommand.GetAddInfo, uses esi	
	SetObject esi	
	.if [esi].FAddInfo	
		invoke GlobalFree, [esi].FAddInfo		
	.endif		
	mov [esi].FAddInfo, $OCall([esi].AddInfoNode::CXMLNode.GetXML)
  	mov eax, [esi].FAddInfo  	
MethodEnd

Method CCyrusCustomCommand.GetReplyAddInfo, uses esi   	 
	SetObject esi	
	.if [esi].FReplyAddInfo	
		invoke GlobalFree, [esi].FReplyAddInfo		
	.endif	
	mov [esi].FReplyAddInfo, $OCall([esi].ReplyAddInfoNode::CXMLNode.GetXML)
  	mov eax, [esi].FReplyAddInfo
MethodEnd

Method CCyrusCustomCommand.AddParam, uses esi, ParamRoot:LPCSTR, ParamName:LPCSTR,\
								ParamType:LPCSTR, ParamValue:LPCSTR
	LOCAL NewNode: CXMLNode								
	LOCAL TempNode: CXMLNode
	LOCAL BinNode: CXMLNode
  	LOCAL i: DWORD 	
  	
  	assume fs:nothing
   	push offset @@OnExcept
   	push fs:[0]
   	mov fs:[0], esp
   								    
	SetObject esi
	LNew NewNode, CXMLNode
	OCall NewNode::CXMLNode.Init
	LNew TempNode, CXMLNode
	OCall TempNode::CXMLNode.Init
	LNew BinNode, CXMLNode
	OCall BinNode::CXMLNode.Init		 
	OCall [esi].RootNode::CXMLNode.GetChildNodesCount
	.if eax == 0        
      	OCall [esi].RootNode::CXMLNode.AddChild, addr szAddInfo   	
      	OCall NewNode::CXMLNode.SetNode, eax
      	OCall [esi].AddInfoNode::CXMLNode.SetNode, eax
    .else     	 	      	    
		.if [esi].AddInfoNode.pNode == NULL    
		  	OCall [esi].RootNode::CXMLNode.GetChildNode, 0
		  	OCall NewNode::CXMLNode.SetNode, eax
		  	OCall [esi].RootNode::CXMLNode.GetChildNodesCount
		  	mov i, eax
		  	.while i      		
		  		OCall [esi].RootNode::CXMLNode.GetName
		  		push eax
		  		invoke lstrcmp, eax, addr szAddInfo  
		  		.if eax == 0
		  			pop eax
		  			invoke GlobalFree, eax
		  			.break
		  		.endif   
		  		pop eax
		  		invoke GlobalFree, eax 		      		      		     
		    	dec i
		    	.if i
		    		OCall NewNode::CXMLNode.NextSibling
		    		push eax
		      		OCall NewNode::CXMLNode.Done
		      		pop eax
		      		OCall NewNode::CXMLNode.SetNode, eax
		      	.endif	
		  	.endw
		  	.if i == 0
		  		OCall NewNode::CXMLNode.Done
		    	OCall [esi].RootNode::CXMLNode.AddChild, addr szAddInfo
		    	OCall NewNode::CXMLNode.SetNode, eax
		    .endif
		    OCall [esi].AddInfoNode::CXMLNode.SetNode, NewNode.pNode  
		.else 
			OCall NewNode::CXMLNode.SetNode, [esi].AddInfoNode.pNode    		 
		.endif  
    .endif         
    .if ParamRoot == NULL    	   
    	OCall NewNode::CXMLNode.AddChild, ParamName     	   	
		OCall NewNode::CXMLNode.SetNode, eax
		OCall [esi].LastNode::CXMLNode.SetNode, eax 	         	
    	OCall BinNode::CXMLNode.SetNode, eax      	      	      
      	.if ParamType      	
        	OCall NewNode::CXMLNode.SetAttribute, addr szType, ParamType
        	invoke lstrcmp, ParamType, addr szBin
        	.if eax == 0        		 
        		OCall esi.AddBinaryNode, addr BinNode, ParamValue    
        	.else
          		OCall NewNode::CXMLNode.SetText, ParamValue
          	.endif			
      	.else
        	OCall NewNode::CXMLNode.SetText, ParamValue
        .endif	            
    .else        
    	OCall [esi].LastNode::CXMLNode.GetName    	 
    	push eax   	   	    	
    	invoke lstrcmp, eax, ParamRoot
      	.if eax == 0  
      		pop eax
      		invoke GlobalFree, eax    		            			      	      			
	    	OCall NewNode::CXMLNode.SetNode, [esi].LastNode.pNode
      	.else   
      		pop eax
      		invoke GlobalFree, eax    		        		
      		OCall TempNode::CXMLNode.SetNode, NewNode.pNode
      		OCall TempNode::CXMLNode.GetChildNode, 0   	
      		OCall NewNode::CXMLNode.SetNode, eax       	        	
        	OCall TempNode::CXMLNode.GetChildNodesCount
        	mov i, eax
        	.while i
        		OCall NewNode::CXMLNode.GetText
        		push eax    	
    			invoke lstrcmp, eax, ParamRoot
      			.if eax == 0
      				pop eax
      				invoke GlobalFree, eax
        	 		.break
        	 	.endif	
        	 	pop eax
      			invoke GlobalFree, eax        	
          		dec i
          		.if i
            		OCall NewNode::CXMLNode.NextSibling
            		push eax
          			OCall NewNode::CXMLNode.Done
          			pop eax
            		OCall NewNode::CXMLNode.SetNode, eax
            	.endif	
        	.endw
        	.if i == 0  
        		OCall NewNode::CXMLNode.Done
        		OCall TempNode::CXMLNode.AddChild, ParamRoot
        		OCall NewNode::CXMLNode.SetNode, eax       		
        		OCall [esi].LastNode::CXMLNode.SetNode, eax      	          		
        	.endif
      	.endif
      	OCall NewNode::CXMLNode.AddChild, ParamName         			
      	OCall NewNode::CXMLNode.SetNode, eax      	      	
      	OCall BinNode::CXMLNode.SetNode, eax      	
      	.if ParamType      	
        	OCall NewNode::CXMLNode.SetAttribute, addr szType, ParamType
        	invoke lstrcmp, ParamType, addr szBin
        	.if eax == 0    
        		OCall esi.AddBinaryNode, addr BinNode, ParamValue    	
        	.else
          		OCall NewNode::CXMLNode.SetText, ParamValue
          	.endif			
      	.else
        	OCall NewNode::CXMLNode.SetText, ParamValue
        .endif	
	.endif    
	.if NewNode.pNode		
		OCall [esi].AddInfoNode::CXMLNode.AppendToChildList, NewNode.pNode		 
	.endif
  	jmp @@Exit
@@OnExcept:
	mov eax, -1		
@@SafeExit:	
@@Exit:      
   	pop fs:[0]
   	add esp, 4  
MethodEnd

Method CCyrusCustomCommand.AddReplyParam, uses esi,	ParamRoot:LPCSTR, ParamName:LPCSTR,\
								ParamType:LPCSTR, ParamValue:LPCSTR   
	LOCAL NewNode: CXMLNode								
	LOCAL TempNode: CXMLNode	
  	LOCAL bstr: BSTR 
  	LOCAL i: DWORD 	
  	
  	assume fs:nothing
   	push offset @@OnExcept
   	push fs:[0]
   	mov fs:[0], esp
   								    
	SetObject esi
	LNew NewNode, CXMLNode
	OCall NewNode::CXMLNode.Init
	LNew TempNode, CXMLNode
	OCall TempNode::CXMLNode.Init	
    OCall [esi].RootNode::CXMLNode.GetChildNodesCount
	.if eax == 0		        
		OCall [esi].RootNode::CXMLNode.AddChild, addr szReplyAddInfo
		OCall NewNode::CXMLNode.SetNode, eax
		OCall [esi].ReplyAddInfoNode::CXMLNode.SetNode, eax    
    .else    
		.if [esi].ReplyAddInfoNode.pNode == NULL		 		   
	      	OCall [esi].RootNode::CXMLNode.GetChildNode, 0
	      	OCall NewNode::CXMLNode.SetNode, eax
	      	OCall [esi].RootNode::CXMLNode.GetChildNodesCount
	      	mov i, eax
	      	.while i       		
	      		OCall [esi].RootNode::CXMLNode.GetName
	      		push eax
	      		invoke lstrcmp, eax, addr szReplyAddInfo
	      		.if eax == 0
	      			pop eax
	      			invoke GlobalFree, eax
	      			.break
	      		.endif
	      		pop eax
	      		invoke GlobalFree, eax    		      		      		     
	        	dec i
	        	.if i
	        		OCall NewNode::CXMLNode.NextSibling
	        		push eax
	        		OCall NewNode::CXMLNode.Done
	        		pop eax
	          		OCall NewNode::CXMLNode.SetNode, eax
	          	.endif	
	      	.endw
	      	.if i == 0
	      		OCall NewNode::CXMLNode.Done
	        	OCall [esi].RootNode::CXMLNode.AddChild, addr szReplyAddInfo
	        	OCall NewNode::CXMLNode.SetNode, eax
	        .endif
	        OCall [esi].ReplyAddInfoNode::CXMLNode.SetNode, NewNode.pNode      	 
	    .else
	    	OCall NewNode::CXMLNode.SetNode, [esi].ReplyAddInfoNode.pNode  	
	    .endif 
    .endif                       
    .if ParamRoot == NULL
    	OCall NewNode::CXMLNode.AddChild, ParamName
    	OCall NewNode::CXMLNode.SetNode, eax
    	OCall [esi].LastNode::CXMLNode.SetNode, eax         	      	     
      	.if ParamType      	
        	OCall NewNode::CXMLNode.SetAttribute, addr szType, ParamType
        .endif	
        OCall NewNode::CXMLNode.SetText, ParamValue          				      	               	    
    .else                  
    	OCall [esi].LastNode::CXMLNode.GetName
    	push eax
    	invoke lstrcmp, eax, ParamRoot    	
      	.if eax == 0
      		pop eax
      		invoke GlobalFree, eax      		      		
	    	OCall NewNode::CXMLNode.SetNode, [esi].LastNode.pNode	    	
      	.else         		
      		pop eax
      		invoke GlobalFree, eax
      		OCall TempNode::CXMLNode.SetNode, NewNode.pNode      		
      		OCall TempNode::CXMLNode.GetChildNode, 0   	
      		OCall NewNode::CXMLNode.SetNode, eax      	        	
        	OCall TempNode::CXMLNode.GetChildNodesCount
        	mov i, eax
        	.while i
        		OCall NewNode::CXMLNode.GetText
        		push eax    	
    			invoke lstrcmp, eax, ParamRoot
      			.if eax == 0
      				pop eax
      				invoke GlobalFree, eax
        	 		.break
        	 	.endif
        	 	pop eax
      			invoke GlobalFree, eax	        	
          		dec i
          		.if i
            		OCall NewNode::CXMLNode.NextSibling
            		push eax
            		OCall NewNode::CXMLNode.Done
            		pop eax
            		OCall NewNode::CXMLNode.SetNode, eax
            	.endif	
        	.endw
        	.if i == 0  
        		OCall NewNode::CXMLNode.Done
        		OCall TempNode::CXMLNode.AddChild, ParamRoot
        		OCall NewNode::CXMLNode.SetNode, eax
        		OCall [esi].LastNode::CXMLNode.SetNode, eax   	          		          		
        	.endif
      	.endif
      	OCall NewNode::CXMLNode.AddChild, ParamName
      	OCall NewNode::CXMLNode.SetNode, eax
      	.if ParamType      	
        	OCall NewNode::CXMLNode.SetAttribute, addr szType, ParamType        	
      	.endif
        OCall NewNode::CXMLNode.SetText, ParamValue        	
	.endif
	.if NewNode.pNode		
		OCall [esi].ReplyAddInfoNode::CXMLNode.AppendToChildList, NewNode.pNode		 
	.endif
 	jmp @@Exit
@@OnExcept:
	mov eax, -1		
@@SafeExit:	
@@Exit:      
   	pop fs:[0]
   	add esp, 4  
MethodEnd

;	//	CCyrusSendCommand Object Implementation
Method CCyrusSendCommand.Init, uses esi   
	SetObject esi	
	ACall esi.Init		 
	OCall [esi].XMLDocument::CXMLDocument.LoadFromXML, addr szCyrusCommandXMLHeader	  
	OCall [esi].XMLDocument.DocumentElement::CXMLNode.AddChild, addr szCommand	
	OCall [esi].RootNode::CXMLNode.SetNode, eax	 	
  	OCall esi.CalculateSize  	  		
MethodEnd

Method CCyrusSendCommand.Done, uses esi   
	SetObject esi
	ACall esi.Clear			
	ACall esi.Done
MethodEnd

Method CCyrusSendCommand.Clear, uses esi   
	SetObject esi	
	ACall esi.Clear		
  	OCall [esi].XMLDocument::CXMLDocument.LoadFromXML, addr szCyrusCommandXMLHeader  
	OCall [esi].XMLDocument.DocumentElement::CXMLNode.AddChild, addr szCommand	
	OCall [esi].RootNode::CXMLNode.SetNode, eax			  	
  	OCall esi.CalculateSize  	
MethodEnd

Method CCyrusSendCommand.SetCommandText, uses esi, Value:LPCSTR 
	SetObject esi	
	.if [esi].FCommandText
		invoke GlobalFree, [esi].FCommandText
	.endif	
	invoke lstrlen, Value
	push eax
	inc eax	
	invoke GlobalAlloc, GPTR, eax	
	mov [esi].FCommandText, eax	
	pop eax	
	invoke memcpy, Value, [esi].FCommandText, eax		
	OCall esi.AddNode, addr szCommandText, Value			
  	OCall esi.CalculateSize
MethodEnd

Method CCyrusSendCommand.SetTarget, uses esi, Value:LPCSTR   
	SetObject esi
	.if [esi].FTarget
		invoke GlobalFree, [esi].FTarget
	.endif		
	invoke lstrlen, Value
	push eax
	inc eax
	invoke GlobalAlloc, GPTR, eax	
	mov [esi].FTarget, eax
	pop eax
	invoke memcpy, Value, [esi].FTarget, eax
	OCall esi.AddNode, addr szTarget, Value
  	OCall esi.CalculateSize
MethodEnd

Method CCyrusSendCommand.SetTargetType, uses esi, Value:LPCSTR    
	SetObject esi
	.if [esi].FTargetType
		invoke GlobalFree, [esi].FTargetType
	.endif	
	invoke lstrlen, Value
	push eax
	inc eax
	invoke GlobalAlloc, GPTR, eax	
	mov [esi].FTargetType, eax
	pop eax
	invoke memcpy, Value, [esi].FTargetType, eax
	OCall esi.SetNodeAttribute, addr szTarget, addr szType, Value
  	OCall esi.CalculateSize	
MethodEnd

Method CCyrusSendCommand.SetSource, uses esi, Value:LPCSTR    
	SetObject esi	
	.if [esi].FSource
		invoke GlobalFree, [esi].FSource
	.endif
	invoke lstrlen, Value
	push eax
	inc eax
	invoke GlobalAlloc, GPTR, eax	
	mov [esi].FSource, eax
	pop eax
	invoke memcpy, Value, [esi].FSource, eax
	OCall esi.AddNode, addr szSource, Value
  	OCall esi.CalculateSize				
MethodEnd

Method CCyrusSendCommand.SetSourceType, uses esi, Value:LPCSTR   
	SetObject esi
	.if [esi].FSourceType
		invoke GlobalFree, [esi].FSourceType
	.endif
	invoke lstrlen, Value
	push eax
	inc eax
	invoke GlobalAlloc, GPTR, eax	
	mov [esi].FSourceType, eax
	pop eax
	invoke memcpy, Value, [esi].FSourceType, eax	
	OCall esi.SetNodeAttribute, addr szSource, addr szType, Value	
  	OCall esi.CalculateSize	  		  	
MethodEnd

Method CCyrusSendCommand.SetReplyReceiver, uses esi, Value:LPCSTR    
	SetObject esi
	.if [esi].FReplyReceiver
		invoke GlobalFree, [esi].FReplyReceiver
	.endif
	invoke lstrlen, Value
	push eax
	inc eax
	invoke GlobalAlloc, GPTR, eax	
	mov [esi].FReplyReceiver, eax
	pop eax
	invoke memcpy, Value, [esi].FReplyReceiver, eax
	OCall esi.AddNode, addr szReplyReceiver, Value
  	OCall esi.CalculateSize		
MethodEnd

Method CCyrusSendCommand.SetReplyReceiverType, uses esi, Value:LPCSTR    
	SetObject esi	
	.if [esi].FReplyReceiverType
		invoke GlobalFree, [esi].FReplyReceiverType
	.endif
	invoke lstrlen, Value
	push eax
	inc eax
	invoke GlobalAlloc, GPTR, eax	
	mov [esi].FReplyReceiverType, eax
	pop eax
	invoke memcpy, Value, [esi].FReplyReceiverType, eax
	OCall esi.SetNodeAttribute, addr szReplyReceiver, addr szType, Value
  	OCall esi.CalculateSize			
MethodEnd

Method CCyrusSendCommand.SetReplyCommandText, uses esi, Value:LPCSTR    
	SetObject esi
	.if [esi].FReplyCommandText
		invoke GlobalFree, [esi].FReplyCommandText
	.endif
	invoke lstrlen, Value
	push eax
	inc eax
	invoke GlobalAlloc, GPTR, eax	
	mov [esi].FReplyCommandText, eax
	pop eax
	invoke memcpy, Value, [esi].FReplyCommandText, eax
	OCall esi.AddNode, addr szReplyCommandText, Value
  	OCall esi.CalculateSize		
MethodEnd

Method CCyrusSendCommand.SetAddInfo, uses esi, Value:LPCSTR    
	SetObject esi
	.if [esi].FAddInfo
		invoke GlobalFree, [esi].FAddInfo
	.endif	
	invoke lstrlen, Value
	push eax
	inc eax
	invoke GlobalAlloc, GPTR, eax		
	mov [esi].FAddInfo, eax
	pop eax
	invoke memcpy, Value, [esi].FAddInfo, eax	
	OCall esi.AddNode, addr szAddInfo, Value
	OCall [esi].AddInfoNode::CXMLNode.SetNode, eax
  	OCall esi.CalculateSize			
MethodEnd

Method CCyrusSendCommand.SetReplyAddInfo, uses esi, Value:LPCSTR    
	SetObject esi	
	.if [esi].FReplyAddInfo
		invoke GlobalFree, [esi].FReplyAddInfo
	.endif
	invoke lstrlen, Value
	push eax
	inc eax
	invoke GlobalAlloc, GPTR, eax		
	mov [esi].FReplyAddInfo, eax
	pop eax
	invoke memcpy, Value, [esi].FReplyAddInfo, eax
	OCall esi.AddNode, addr szReplyAddInfo, Value
	OCall [esi].ReplyAddInfoNode::CXMLNode.SetNode, eax	
  	OCall esi.CalculateSize				
MethodEnd

Method CCyrusSendCommand.SetSize, uses esi, Value:DWORD
	LOCAL temp[16]: BYTE
	LOCAL pBuff: DWORD	  	 
	SetObject esi	
	mov ebx, Value
	invoke dwtoa, Value, addr temp		
	invoke lstrlen, addr temp
	add ebx, eax
	mov pBuff, $OCall([esi].XMLDocument.DocumentElement::CXMLNode.GetAttribute, addr szSize)	
	invoke lstrlen, pBuff
	sub ebx, eax	 
	invoke GlobalFree, pBuff	
	invoke dwtoa, ebx, addr temp		
	OCall [esi].XMLDocument.DocumentElement::CXMLNode.SetAttribute, addr szSize, addr temp
MethodEnd

Method CCyrusSendCommand.GetSize, uses esi   		
	LOCAL pBuff: DWORD		 
	SetObject esi		
	mov pBuff, $OCall([esi].XMLDocument.DocumentElement::CXMLNode.GetAttribute, addr szSize)		
	invoke atodw, pBuff
	push eax 
	invoke GlobalFree, pBuff	
	pop eax	
MethodEnd

Method CCyrusSendCommand.CalculateSize, uses esi
	LOCAL pBuff: DWORD 	 
	SetObject esi		
	mov pBuff, $OCall([esi].XMLDocument::CXMLDocument.GetXML)		
	invoke lstrlen, pBuff				
	OCall esi.SetSize, eax	
	invoke GlobalFree, pBuff	
MethodEnd

Method CCyrusSendCommand.AddParameter, uses esi, ParamRoot:LPCSTR, ParamName:LPCSTR,\
								ParamType:LPCSTR, ParamValue:LPCSTR   
	SetObject esi	
	OCall esi.AddParam, ParamRoot, ParamName, ParamType, ParamValue
  	OCall esi.CalculateSize	
MethodEnd

Method CCyrusSendCommand.AddReplyParameter, uses esi, ParamRoot:LPCSTR, ParamName:LPCSTR,\
								ParamType:LPCSTR, ParamValue:LPCSTR   
	SetObject esi	
	OCall esi.AddReplyParam, ParamRoot, ParamName, ParamType, ParamValue
  	OCall esi.CalculateSize		
MethodEnd

;	//	CCyrusRecvCommand Object Implementation
Method CCyrusRecvCommand.Init, uses esi, InitMsg:LPCSTR
	LOCAL hHeap: DWORD
	SetObject esi
	ACall esi.Init		
	OCall esi.ParseMsg, InitMsg	
MethodEnd

Method CCyrusRecvCommand.Done, uses esi
	SetObject esi			
	OCall esi.Clear			
	ACall esi.Done				
MethodEnd

Method CCyrusRecvCommand.Clear, uses esi ebx edx
	LOCAL i: DWORD
	SetObject esi			
	ACall esi.Clear						
	mov ebx, [esi].FParameters
	assume ebx: ptr CyrusCommandParam
	m2m i, [esi].FParametersCount
	.while i									
		.if [ebx].ParamName 			
			invoke GlobalFree, [ebx].ParamName
		.endif
		invoke lstrcmp, [ebx].ParamType, addr szNestedParam
		.if eax
			.if [ebx].ParamValue
				invoke GlobalFree, [ebx].ParamValue
			.endif
		.endif	
		.if [ebx].ParamType 
			invoke GlobalFree, [ebx].ParamType			
		.endif			
		add ebx, sizeof CyrusCommandParam
		dec i 
	.endw		
  	invoke SetLength, addr [esi].FParameters, 0, 0  	
  	mov [esi].FParametersCount, 0    	
  	mov ebx, [esi].FReplyParameters
	m2m i, [esi].FReplyParametersCount	
	.while i								
		.if [ebx].ParamName 
			invoke GlobalFree, [ebx].ParamName
		.endif
		invoke lstrcmp, [ebx].ParamType, addr szNestedParam
		.if eax
			.if [ebx].ParamValue
				invoke GlobalFree, [ebx].ParamValue
			.endif
		.endif
		.if [ebx].ParamType 
			invoke GlobalFree, [ebx].ParamType			
		.endif			
		add ebx, sizeof CyrusCommandParam
		dec i 
	.endw		
	assume ebx:nothing	
  	invoke SetLength, addr [esi].FReplyParameters, 0, 0
  	mov [esi].FReplyParametersCount, 0  	
  	mov [esi].FSize, 0
  	mov [esi].FIsCyrusCommand, FALSE
  	mov [esi].FHasCyrusTags, FALSE   	  	
MethodEnd

Method CCyrusRecvCommand.SetMsg, uses esi, Value:LPCSTR
	SetObject esi
	OCall esi.ParseMsg, Value
MethodEnd

Method CCyrusRecvCommand.GetParametersCount, uses esi
	SetObject esi
	mov eax, [esi].FParametersCount
MethodEnd

Method CCyrusRecvCommand.GetReplyParametersCount, uses esi
	SetObject esi
	mov eax, [esi].FReplyParametersCount
MethodEnd

Method CCyrusRecvCommand.GetParameters, uses esi
	SetObject esi
	mov eax, [esi].FParameters
MethodEnd

Method CCyrusRecvCommand.GetReplyParameters, uses esi
	SetObject esi
	mov eax, [esi].FReplyParameters
MethodEnd

Method CCyrusRecvCommand.GetSize, uses esi
	SetObject esi
	mov eax, [esi].FSize
MethodEnd

Method CCyrusRecvCommand.IsCyrusCommand, uses esi
	SetObject esi
	xor eax, eax
	mov al, [esi].FIsCyrusCommand
MethodEnd

Method CCyrusRecvCommand.HasCyrusTags, uses esi
	SetObject esi
	xor eax, eax
	mov al, [esi].FHasCyrusTags
MethodEnd

Method CCyrusRecvCommand.ParseMsg, uses esi ebx, Value:LPCSTR
  	LOCAL i: DWORD
  	LOCAL j: DWORD
  	LOCAL strValue: LPCSTR  
  	LOCAL NewNode: CXMLNode	
	
	assume fs:nothing
   	push offset @@OnExcept
   	push fs:[0]
   	mov fs:[0], esp
   	
	SetObject esi
	LNew NewNode, CXMLNode
	OCall NewNode::CXMLNode.Init		
	invoke PosStr, Value, addr szCyrusCommandsFlag
    .if eax == 1    	
     	mov [esi].FHasCyrusTags, TRUE
     	invoke lstrlen, Value
     	inc eax
     	invoke GlobalAlloc, GPTR, eax
     	mov strValue, eax 
     	invoke lstrcpy, strValue, Value
     	invoke PosStr, strValue, StrM('"')     	  
     	invoke DeleteStr, strValue, 1, eax
     	invoke PosStr, strValue, StrM('"')
     	dec eax
     	invoke CopyStr, strValue, strValue, 1, eax   
     	invoke atodw, strValue
     	mov ebx, eax 
     	invoke lstrlen, Value
      	.if ebx != eax      		
      		invoke GlobalFree, strValue
        	mov [esi].FIsCyrusCommand, FALSE
        	jmp @@Exit
      	.endif        	  
    .else    
    	invoke GlobalFree, strValue
      	mov [esi].FHasCyrusTags, FALSE
      	mov [esi].FIsCyrusCommand, FALSE
      	jmp @@Exit
    .endif
    OCall [esi].XMLDocument::CXMLDocument.LoadFromXML, Value        
    invoke lstrcmp, $OCall([esi].XMLDocument.DocumentElement::CXMLNode.GetName),\ 
    							addr szCyrusCommands
    .if eax
     	mov [esi].FIsCyrusCommand, FALSE
    .else
    	OCall [esi].XMLDocument.DocumentElement::CXMLNode.GetAttribute, addr szSize
      	mov [esi].FSize, eax
      	OCall [esi].XMLDocument.DocumentElement::CXMLNode.GetChildNode, 0	
      	OCall NewNode::CXMLNode.SetNode, eax					
      	invoke lstrcmp, $OCall(NewNode::CXMLNode.GetName), addr szCommand						
      	.if eax == 0
        	mov [esi].FIsCyrusCommand, TRUE
        	OCall NewNode::CXMLNode.Done
        	OCall [esi].XMLDocument.DocumentElement::CXMLNode.GetChildNode, 0
        	OCall [esi].RootNode::CXMLNode.SetNode, eax        	
			OCall [esi].RootNode::CXMLNode.GetChildNode, 0
			OCall NewNode::CXMLNode.SetNode, eax
			mov i, $OCall([esi].RootNode::CXMLNode.GetChildNodesCount)       	        	
        	.while i   
        		OCall NewNode::CXMLNode.GetName
          		mov strValue, eax          		  		
          		invoke lstrcmp, strValue, addr szSource
          		.if eax == 0            			
          			mov [esi].FSource, $OCall(NewNode::CXMLNode.GetText)          			
            		mov [esi].FSourceType, $OCall(NewNode::CXMLNode.GetAttribute, addr szType)            		            		
            		jmp @F            	            			
          		.endif
          		invoke lstrcmp, strValue, addr szTarget
          		.if eax == 0  
          			mov [esi].FTarget, $OCall(NewNode::CXMLNode.GetText)
            		mov [esi].FTargetType, $OCall(NewNode::CXMLNode.GetAttribute, addr szType)            		
            		jmp @F            	            			
          		.endif
          		invoke lstrcmp, strValue, addr szReplyReceiver
          		.if eax == 0  
          			mov [esi].FReplyReceiver, $OCall(NewNode::CXMLNode.GetText)
            		mov [esi].FReplyReceiverType, $OCall(NewNode::CXMLNode.GetAttribute, addr szType)            		
            		jmp @F            	            			
          		.endif
          		invoke lstrcmp, strValue, addr szCommandText
          		.if eax == 0  
          			mov [esi].FCommandText, $OCall(NewNode::CXMLNode.GetText)            		            		
            		jmp @F            	            			
          		.endif
          		invoke lstrcmp, strValue, addr szReplyCommandText
          		.if eax == 0  
          			mov [esi].FReplyCommandText, $OCall(NewNode::CXMLNode.GetText)            		            		
            		jmp @F            	            			
          		.endif             		
          		invoke lstrcmp, strValue, addr szAddInfo
          		.if eax == 0    
          			OCall [esi].AddInfoNode::CXMLNode.SetNode, NewNode.pNode           			
            		.if [esi].FAddInfo	  
  						invoke GlobalFree, [esi].FAddInfo
  						mov [esi].FAddInfo, NULL
  					.endif
  					OCall [esi].AddInfoNode::CXMLNode.GetChildNode, 0
            		OCall NewNode::CXMLNode.SetNode, eax
            		mov j, $OCall([esi].AddInfoNode::CXMLNode.GetChildNodesCount)             		
            		.while j
            			invoke lstrcat, [esi].FAddInfo, $OCall(NewNode::CXMLNode.GetXML)            		              			
              			dec j
              			.if j
                			OCall NewNode::CXMLNode.NextSibling
                			push eax
            				OCall NewNode::CXMLNode.Done
            				pop eax
          					OCall NewNode::CXMLNode.SetNode, eax
                		.endif	
            		.endw            		
          			OCall NewNode::CXMLNode.SetNode, [esi].AddInfoNode.pNode
          			OCall NewNode::CXMLNode.GetChildNodesCount
		            .if eax		            		 
        		    	OCall esi.ParseNestedParam, addr NewNode, addr [esi].FParameters,\
        		    				addr [esi].FParametersCount
        		    .endif	          			            		            		
            		jmp @F     		
          		.endif
				invoke lstrcmp, strValue, addr szReplyAddInfo
          		.if eax == 0    
          			OCall [esi].ReplyAddInfoNode::CXMLNode.SetNode, NewNode.pNode           			
            		.if [esi].FReplyAddInfo	  
  						invoke GlobalFree, [esi].FReplyAddInfo
  						mov [esi].FReplyAddInfo, NULL
  					.endif
  					OCall [esi].ReplyAddInfoNode::CXMLNode.GetChildNode, 0
            		OCall NewNode::CXMLNode.SetNode, eax
            		mov j, $OCall([esi].ReplyAddInfoNode::CXMLNode.GetChildNodesCount)             		
            		.while j
            			invoke lstrcat, [esi].FReplyAddInfo, $OCall(NewNode::CXMLNode.GetXML)            		              			
              			dec j
              			.if j
                			OCall NewNode::CXMLNode.NextSibling
                			push eax
            				OCall NewNode::CXMLNode.Done
            				pop eax
          					OCall NewNode::CXMLNode.SetNode, eax
                		.endif	
            		.endw            		
          			OCall NewNode::CXMLNode.SetNode, [esi].ReplyAddInfoNode.pNode
          			OCall NewNode::CXMLNode.GetChildNodesCount
		            .if eax
        		    	OCall esi.ParseNestedParam, addr NewNode, addr [esi].FReplyParameters,\
        		    				addr [esi].FReplyParametersCount
        		    .endif	          			            		            		
            		jmp @F     		
          		.endif          		
			@@:
				invoke GlobalFree, strValue
          		dec i
          		.if i
            		OCall NewNode::CXMLNode.NextSibling
            		push eax
            		OCall NewNode::CXMLNode.Done
            		pop eax
          			OCall NewNode::CXMLNode.SetNode, eax
          		.else
          			OCall NewNode::CXMLNode.Done	
                .endif	
        	.endw
      	.else
      		mov [esi].FIsCyrusCommand, FALSE
      		OCall NewNode::CXMLNode.Done
        .endif	
    .endif            
  	jmp @@Exit
@@OnExcept:    	
	mov eax, -1		
@@SafeExit:	
@@Exit:      
   	pop fs:[0]
   	add esp, 4  
MethodEnd

Method CCyrusRecvCommand.ParseNestedParam, uses esi ebx ecx edx edi, InitRootNode:PXMLNode,\
										ppParams:DWORD, pParamsCount:PDWORD
	LOCAL NewNode: CXMLNode										
  	LOCAL TempNode: CXMLNode
  	LOCAL BinNode: CXMLNode
  	LOCAL i: DWORD
  	LOCAL j: DWORD
  	LOCAL jMax: DWORD
  	LOCAL rb: PReplacedBytes
  	LOCAL strTemp: LPCSTR	
  	LOCAL dwParamsCount: DWORD
  	LOCAL temp[16]: DWORD
  										
	SetObject esi
	LNew NewNode, CXMLNode
	OCall NewNode::CXMLNode.Init
	LNew TempNode, CXMLNode
	OCall TempNode::CXMLNode.Init
	LNew BinNode, CXMLNode
	OCall BinNode::CXMLNode.Init
		
	OCall InitRootNode::CXMLNode.GetChildNode, 0
    OCall TempNode::CXMLNode.SetNode, eax
    assume ebx:ptr CyrusCommandParam 	  	
  	mov i, $OCall(InitRootNode::CXMLNode.GetChildNodesCount)
  	.while i > 0  		 
  		mov eax, pParamsCount
  		mov edx, dword ptr [eax]  		
  		inc edx  		  		  		    	    
  		mov dword ptr [eax], edx
  		mov dwParamsCount, edx    				   	
    	invoke SetLength, ppParams, sizeof CyrusCommandParam, dwParamsCount  
    	mov ebx, ppParams
    	mov dword ptr [ebx], eax   	
    	mov eax, dwParamsCount    	
    	dec eax
    	mov edx, sizeof CyrusCommandParam
    	mul edx        		    	
    	mov ebx, dword ptr [ebx]
    	add ebx, eax    	   	 

    	mov [ebx].ParamName, $OCall(TempNode::CXMLNode.GetName)    	   		    	
  		.if $OCall(TempNode::CXMLNode.HasAttribute, addr szType)  			
  			OCall TempNode::CXMLNode.GetAttribute, addr szType
  			mov strTemp, eax   			
  			invoke lstrcmp, strTemp, addr szBin      		
        	.if eax == 0
        		invoke GlobalFree, strTemp
        		OCall TempNode::CXMLNode.GetChildNode, 0
        		push eax
        		OCall TempNode::CXMLNode.Done
        		pop eax
        		OCall TempNode::CXMLNode.SetNode, eax
        		OCall BinNode::CXMLNode.SetNode, eax   
        		OCall TempNode::CXMLNode.GetName
        		mov strTemp, eax
        		OCall TempNode::CXMLNode.HasChildNodes
        		mov edx, eax
        		invoke lstrcmp, strTemp, addr szReplacedBytes            		
          		.if (eax == 0) && edx
          			invoke GlobalFree, strTemp              			      		
            		mov jMax, $OCall(TempNode::CXMLNode.GetChildNodesCount)
            		dec jMax
            		OCall TempNode::CXMLNode.GetChildNode, 0           		
        			OCall TempNode::CXMLNode.SetNode, eax
            		mov j, 0
              		mov edx, jMax
              		mov rb, NULL              		
            		.while j <= edx             			            			             	            			          				            			 
            			invoke SetLength, addr rb, sizeof ReplacedByte, j + 1            			           			          			
            			mov strTemp, $OCall(TempNode::CXMLNode.GetName)
            			invoke lstrlen, strTemp
            			dec eax
            			invoke CopyStr, strTemp, strTemp, 2, eax
            			mov edi, rb
                		mov eax, sizeof ReplacedByte
                		mov ecx, j
                		mul ecx
                		add edi, eax   
            			invoke atodw, strTemp            		            			 
						mov (ReplacedByte ptr [edi]).iIndex, eax						    
						invoke GlobalFree, strTemp						
						mov strTemp, $OCall(TempNode::CXMLNode.GetText)																	  
						invoke atodw, strTemp   									    
						mov (ReplacedByte ptr [edi]).chOriginal, al
						invoke GlobalFree, strTemp						
                		mov eax, j                		   
						.if eax != jMax						         			
              				OCall TempNode::CXMLNode.NextSibling
              				push eax
              				OCall TempNode::CXMLNode.Done
              				pop eax
    						OCall TempNode::CXMLNode.SetNode, eax
    					.endif 					  
    					inc j 
    					mov edx, jMax 					              		
                	.endw	
                	OCall TempNode::CXMLNode.Done              			            		
            	.else
            		invoke GlobalFree, strTemp	
          		.endif           		           		        	
          		OCall BinNode::CXMLNode.NextSibling
    			OCall TempNode::CXMLNode.SetNode, eax
    			OCall TempNode::CXMLNode.GetName
        		mov strTemp, eax
        		invoke lstrcmp, strTemp, addr szBinaryStream
        		.if eax == 0   
        			invoke GlobalFree, strTemp
        			OCall TempNode::CXMLNode.GetText
        			mov strTemp, eax        			
            		.if eax             			             			              			
              			.if jMax
              				mov j, 0
              				mov ecx, jMax
                			.while j <= ecx
                				mov edi, rb
                				mov eax, sizeof ReplacedByte
                				mov edx, j
                				mul edx
                				add edi, eax 
                				mov eax, strTemp
                				add eax, (ReplacedByte ptr [edi]).iIndex
                				mov dl, (ReplacedByte ptr [edi]).chOriginal
                				mov byte ptr [eax], dl
                				inc j
                				mov ecx, jMax
                			.endw	
                		.endif		                		              			
            		.endif
            		m2m [ebx].ParamValue, strTemp            		
            	.else
            		invoke GlobalFree, strTemp			
          		.endif
          		OCall TempNode::CXMLNode.Done
          		OCall BinNode::CXMLNode.GetParentNode
          		OCall TempNode::CXMLNode.SetNode, eax
          		OCall BinNode::CXMLNode.Done          		
          		jmp @@Next          		        	
        	.else
        		invoke GlobalFree, strTemp
        		mov [ebx].ParamType, $OCall(TempNode::CXMLNode.GetAttribute, addr szType)        		
    		.endif	        	          	
      	.endif      	
      	mov edx, $OCall(TempNode::CXMLNode.IsTextElement)     	
      	OCall TempNode::CXMLNode.GetChildNodesCount
      	.if eax == 0
	        mov [ebx].ParamValue, NULL 
    	.elseif (eax == 1) && edx    	     			       	
	       	mov [ebx].ParamValue, $OCall(TempNode::CXMLNode.GetText)	       
	    .else    
      		mov [ebx].ParamValue, $OCall(TempNode::CXMLNode.GetChildNodesCount)
      		invoke lstrlen, addr szNestedParam
      		inc eax
      		invoke GlobalAlloc, GPTR, eax
      		mov [ebx].ParamType, eax
      		invoke lstrcpy, eax, addr szNestedParam       		    		  	        	        	
        	OCall esi.ParseNestedParam, addr TempNode, ppParams, pParamsCount
      	.endif 
 	@@Next:	  		    	
   		dec i
   		.if i
    		OCall TempNode::CXMLNode.NextSibling
    		push eax
    		OCall TempNode::CXMLNode.Done
    		pop eax
    		OCall TempNode::CXMLNode.SetNode, eax
    	.endif	
    .endw	
    OCall TempNode::CXMLNode.Done
    assume ebx:nothing
MethodEnd
