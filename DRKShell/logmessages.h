 // Daphne.DRKShell logmessages.h
 // This is the header section.
#ifndef __MESSAGES_H__
#define __MESSAGES_H__

 // The following are the categories of events.
//
//  Values are 32 bit values laid out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//
#define FACILITY_SYSTEM                  0x0
#define FACILITY_STUBS                   0x3
#define FACILITY_RUNTIME                 0x2
#define FACILITY_IO_ERROR_CODE           0x4


//
// Define the severity codes
//
#define STATUS_SEVERITY_WARNING          0x2
#define STATUS_SEVERITY_SUCCESS          0x0
#define STATUS_SEVERITY_INFORMATIONAL    0x1
#define STATUS_SEVERITY_ERROR            0x3


//
// MessageId: BASE_CATEGORY
//
// MessageText:
//
// Normal Events
//
#define BASE_CATEGORY                    ((WORD)0x00000001L)

//
// MessageId: NETWORK_CATEGORY
//
// MessageText:
//
// Network Events
//
#define NETWORK_CATEGORY                 ((WORD)0x00000002L)

//
// MessageId: UI_CATEGORY
//
// MessageText:
//
// UI Events
//
#define UI_CATEGORY                      ((WORD)0x00000003L)

 // The following are the message definitions.
//
// MessageId: MSG_CLIPBOARD_ERROR
//
// MessageText:
//
// There was a problem accessing the system clipboard: %1.
//
#define MSG_CLIPBOARD_ERROR              ((DWORD)0x80020100L)

//
// MessageId: MSG_RESTART_MNGR_ERROR
//
// MessageText:
//
// There was a problem accessing the Restart Manager: %1.
//
#define MSG_RESTART_MNGR_ERROR           ((DWORD)0x80020101L)


#endif  //__MESSAGES_H__
