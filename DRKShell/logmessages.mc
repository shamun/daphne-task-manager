; // Daphne.DRKShell logmessages.h
; // This is the header section.

;#ifndef __MESSAGES_H__
;#define __MESSAGES_H__
;


SeverityNames=(Success=0x0:STATUS_SEVERITY_SUCCESS
               Informational=0x1:STATUS_SEVERITY_INFORMATIONAL
               Warning=0x2:STATUS_SEVERITY_WARNING
               Error=0x3:STATUS_SEVERITY_ERROR
              )


FacilityNames=(System=0x0:FACILITY_SYSTEM
               Runtime=0x2:FACILITY_RUNTIME
               Stubs=0x3:FACILITY_STUBS
               Io=0x4:FACILITY_IO_ERROR_CODE
              )

LanguageNames=(English=0x409:MSG00409)


; // The following are the categories of events.

MessageIdTypedef=WORD

MessageId=0x1
SymbolicName=BASE_CATEGORY
Language=English
Normal Events
.

MessageId=0x2
SymbolicName=NETWORK_CATEGORY
Language=English
Network Events
.

MessageId=0x3
SymbolicName=UI_CATEGORY
Language=English
UI Events
.


; // The following are the message definitions.

MessageIdTypedef=DWORD

MessageId=0x100
Severity=Warning
Facility=Runtime
SymbolicName=MSG_CLIPBOARD_ERROR
Language=English
There was a problem accessing the system clipboard: %1.
.

MessageId=0x101
Severity=Warning
Facility=Runtime
SymbolicName=MSG_RESTART_MNGR_ERROR
Language=English
There was a problem accessing the Restart Manager: %1.
.

;
;#endif  //__MESSAGES_H__
;