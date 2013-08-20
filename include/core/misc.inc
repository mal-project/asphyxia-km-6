;-----------------------------------------------------------------------
.code
    hash    proc    lpmessage, lpsha1
        invoke  sha1_init, lpsha1
        invoke  xszlen, lpmessage
        invoke  sha1_update, lpsha1, lpmessage, eax
        invoke  sha1_final, lpsha1
        
        ret
    hash    endp

;-----------------------------------------------------------------------
    verify  proc    hname, hserial1, hserial2
        local   hserial:hBIG
        pushad
        
        invoke  big_create, 0
        mov     hserial, eax

        ; big_mul ( hName, 2 )
        invoke  big_var32, eax, 2
        invoke  big_mul, hname, hserial, hname

        ; big_mul ( hName, hName )
        invoke  big_mul, hname, hname, hname
        
        ; big_mul ( hSerial1, hSerial1 )
        invoke  big_mul, hserial1, hserial1, hserial1

        ; big_mul ( hSerial2, hSerial2 )
        invoke  big_mul, hserial2, hserial2, hserial2
        
        ; big_add ( hName, hSerial1 )
        invoke  big_add, hname, hserial1, hserial1

        ; 0 = (lpbig1==lpbig2), 1 = (lpbig1>lpbig2), -1 = (lpbig1<lpbig2)
        invoke  big_compare, hserial1, hserial2
        .if     !eax
            return  TRUE
        .else
            return  FALSE
        .endif
        
        invoke  big_destroy, hserial
        
        popad
        ret
    verify  endp

;-----------------------------------------------------------------------
    verify_registration proc    lpszname, lpregistration
        local   hname, hserial1, hserial2:hBIG, sha1:ssha1_ctx

        pushad
        mov     esi, lpregistration
        .if     byte ptr (sregistration ptr [esi]).part1.dblen <= 28h && byte ptr (sregistration ptr [esi]).part2.dblen <= 28h

            invoke  big_create_array, addr hserial2, 3

            invoke  hash, lpszname, addr sha1
            invoke  big_bytes_to_big, eax, 20, hname
            
            mov     esi, lpregistration
            invoke  big_bytes_to_big, addr (sregistration ptr [esi]).part1.hbig, (sregistration ptr [esi]).part2.dblen, hserial1
            invoke  big_bytes_to_big, addr (sregistration ptr [esi]).part2.hbig, (sregistration ptr [esi]).part2.dblen, hserial2
            
            invoke  verify, hname, hserial1, hserial2
            return  eax

            invoke  big_destroy_array, addr hserial2, 3

        .else
            return  FALSE
        .endif

        popad
        ret
    verify_registration endp
    
;-----------------------------------------------------------------------
    print_random_activation proc    lpbufferin, dwlen, lpbufferout
        local   hrandom:hBIG
        
        invoke  big_create, 0
        mov     hrandom, eax
        
        invoke  big_bytes_to_big, lpbufferin, dwlen, hrandom
        invoke  big_cotstr, hrandom, lpbufferout
        
        invoke  big_destroy, hrandom

        ret
    print_random_activation endp

;-----------------------------------------------------------------------
    validate_format proc   lpszstr
        pushad
        mov     esi, lpszstr

        .while  byte ptr [esi] != 0
            .if byte ptr [esi] >= 'A' && byte ptr [esi] <= 'F' || byte ptr [esi] >= '0' && byte ptr [esi] <= '9'
           
            .else
                .break
            .endif
            
            inc     esi
        
        .endw

        .if     byte ptr [esi] == 0
            sub     esi, lpszstr
            return  esi
        
        .else
            return  FALSE

        .endif

        popad
        ret
    validate_format endp

;-----------------------------------------------------------------------
    create_random_activation    proc    lpbufferout
        local   cProvider:HANDLE
        pushad

        invoke  CryptAcquireContext, addr cProvider, NULL, NULL, 1, NULL
        invoke  CryptGenRandom, cProvider, 20, lpbufferout
        invoke  CryptReleaseContext, cProvider, 0

        popad
        ret
    create_random_activation    endp

;-----------------------------------------------------------------------
