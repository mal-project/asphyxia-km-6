; 10/10/2009 -----------------------------------------------------------
; +------------u  n  t  i  l----r  e  a  c  h----v  o  i  d------------¦
; ¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦
; ¦            ¦¦        _   ¦¦            ¦¦           ¯¦¦       ¯    ¦
; ¦  ________  ¦¦___      ¯¯¯¦¦  ________  ¦¦            ¦¦¦_        ¯¦¦
; ¦     _      ¦¦   ¯        ¦¦           _¦¦        _   ¦¦   _        ¦
; ¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦
; a         s         p         h        y         x         i         a
;
; ¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦
;
; in memoriam of l.
; smiling in another plane of existence, perhaps...

.code
;-----------------------------------------------------------------------
;(sha1(szname)*2)^2 + serial.part1^2 == serial.part2^2
;(sha1(szname)*2)*(sha1(szname)*2) + serial.part1*serial.part1 == serial.part2*serial.part2
    include core\misc.inc

    registration    proc    hWnd
        local   szname[MAX_NAME_LENGTH+1]:byte, szregistration[200]:byte, hregistration:sregistration
        pushad
        
        .while  (1)
            invoke  WaitForSingleObject, hEvent, -1

            invoke  SendDlgItemMessage, hWnd, IDE_NAME, WM_GETTEXT, sizeof szname, addr szname
            .if     eax >= MIN_NAME_LENGTH && eax <= MAX_NAME_LENGTH

                invoke  SendDlgItemMessage, hWnd, IDE_REGISTRATION, WM_GETTEXT, sizeof szregistration, addr szregistration
                invoke  validate_format, addr szregistration
                .if     eax >= MIN_SERIAL_LENGTH && eax <= MAX_SERIAL_LENGTH
                    
                    invoke  xfill, 0, addr hregistration, sizeof sregistration
                    invoke  xbase256, addr szregistration, addr hregistration

                    invoke  blowfish_init, addr random, 20
                    invoke  blowfish_decrypt, addr hregistration, sizeof sregistration, 0
           
                    invoke  verify_registration, addr szname, addr hregistration
                    
                    .if     eax == TRUE
                        invoke  PostMessage, hWnd, WM_DEFEATED, 0, 0
                    .endif

                .endif
            
            .endif
            
        .endw
        
        popad
        ret
    registration    endp

;-----------------------------------------------------------------------
    initialize  proc    hWnd:HWND
        local   dwlen:dword, szusername[100]:byte, szrandom[100]:byte
        pushad

        invoke  create_random_activation, addr random
        
        invoke  print_random_activation, addr random, 20, addr szrandom
        invoke  SendDlgItemMessage, hWnd, IDE_ACTIVATION, WM_SETTEXT, 0, addr szrandom

        ; Fetching and displaying user name in field "Name"
        mov     dwlen, MAX_NAME_LENGTH
        invoke  GetUserName, addr szusername, addr dwlen
        invoke  SendDlgItemMessage, hWnd, IDE_NAME, WM_SETTEXT, 0, addr szusername
        
        ; Setting up object to activate the thread below
        invoke  CreateEvent, 0, 0, 0, 0
        mov     hEvent, eax

        ; Creates registration basic check thread
        invoke  CreateThread, NULL, NULL, addr registration, hWnd, NULL, NULL

        popad
        ret
    initialize  endp

;-----------------------------------------------------------------------
    deinitialize    proc
        
        ret
    deinitialize    endp

;-----------------------------------------------------------------------
