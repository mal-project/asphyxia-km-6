;-----------------------------------------------------------------------
include     includes.inc

;-----------------------------------------------------------------------
HideDlgProc     proto   :HWND,:UINT,:WPARAM,:LPARAM
HelpDlgProc     proto   :HWND,:UINT,:WPARAM,:LPARAM
MainDlgProc     proto   :HWND,:UINT,:WPARAM,:LPARAM

;-----------------------------------------------------------------------
initialize          proto   :HWND
registration        proto   :dword
base256             proto   :dword, :dword

;-----------------------------------------------------------------------
MIN_NAME_LENGTH     equ 5
MAX_NAME_LENGTH     equ 64

MIN_SERIAL_LENGTH   equ 175
MAX_SERIAL_LENGTH   equ 177

WM_DEFEATED equ WM_USER + 1337

;-----------------------------------------------------------------------
CRLF    equ 13, 10
TAB     equ 9

;-----------------------------------------------------------------------
sbig    struct
    dblen   dd  ?
    hbig    db  40  dup(?)
sbig    ends

sregistration   struct
    part1   sbig    <>
    part2   sbig    <>
sregistration   ends

;-----------------------------------------------------------------------
.const   
    szTitle     db "Congratulations!", 0
    szMessage   db "You just defeated this protection.", CRLF
                db "Now you should share your knowledge with others.", CRLF
                db "Make a tutorial on how you reverse it.", 0

;-----------------------------------------------------------------------
.data

;-----------------------------------------------------------------------    
.data?
    hInst   HINSTANCE   ?
    hEvent  HANDLE      ?

    random    byte  20  dup(?)

;-----------------------------------------------------------------------
