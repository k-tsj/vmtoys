/*
  Copyright (C) 2004-2011 Kazuki Tsujimoto, All rights reserved.
  Copyright (c) 2006 Ken Kato(vmw)

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

  1. Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.

  3. Neither the name of the authors nor the names of its contributors
     may be used to endorse or promote products derived from this
     software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <signal.h>
#include <setjmp.h>
#include "vmw_door.h"

static sigjmp_buf sigsegvbuffer;
static void sigsegvhandler(int sig) { siglongjmp(sigsegvbuffer, sig); }
    
void vmw_door(vmw_door_t *r)
{
    int eax, ebx, ecx, edx;

    eax = r->eax;
    ebx = r->ebx;
    ecx = r->ecx;
    edx = r->edx;

    __asm__ __volatile__("inl %%dx"
                         : "=a"(eax), "=c"(ecx), "=d"(edx),
                           "=b"(ebx)
                         : "0"(eax), "1"(ecx), "2"(edx),
                           "3"(ebx)
                         : "memory"
                         );

    r->eax = eax;
    r->ebx = ebx;
    r->ecx = ecx;
    r->edx = edx;
}


int vmw_check_env(void) {
    vmw_door_t regs = { VMW_ADDR, 0, VMW_CMD_GET_VERSION, VMW_PORT };
    struct sigaction action, oldaction;

    /* setup signal handler to catch segmentation violation */
    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_RESTART;
    action.sa_handler = sigsegvhandler;
    sigaction(SIGSEGV, &action, &oldaction);
    if (sigsetjmp(sigsegvbuffer, 1) != 0) {
        sigaction(SIGSEGV, &oldaction, 0);
        fprintf(stderr, "Not running on VMware");
        return 0;
    }
    vmw_door(&regs);
    sigaction(SIGSEGV, &oldaction, 0);

    if (regs.ebx != VMW_ADDR) {
        fprintf(stderr, "Incorrect virtual machine version "
                "%x (should be %x)\n", regs.ebx, VMW_ADDR);
        return 0;
    }

    if (regs.eax != VMW_VERS) {
        fprintf(stderr, "VMware software version %d (should be %d)\n",
                regs.eax, VMW_VERS);
        return 0;
    }

    return 1;
}
