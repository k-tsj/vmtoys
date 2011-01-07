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


#define ADJUST_INTERVAL_SEC 30

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <syslog.h>
#include "vmw_door.h"

static void log_exit(const char *fmt, ...);

int main(int argc, char *argv[])
{
    if (vmw_check_env()) {
        if (daemon(0, 0) < 0) {
            fprintf(stderr, "daemon(3) failed: %s\n", strerror(errno));
            return 1;
        }

        while (1) {
            struct timespec timespec;
            vmw_door_t regs = { VMW_ADDR, 0, VMW_CMD_GET_TIME, VMW_PORT };
            vmw_door(&regs);

            timespec.tv_sec = regs.eax;
            timespec.tv_nsec = regs.ebx * 1000L + regs.ecx / 1000;

            if (clock_settime(CLOCK_REALTIME, &timespec) < 0) {
                log_exit("clock_settime(3) failed: %s", strerror(errno));
            }

            sleep(ADJUST_INTERVAL_SEC);
        }
    }
    return 1;
}


static void log_exit(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    vsyslog(LOG_ERR, fmt, ap);
    va_end(ap);
    exit(1);
}

