/*
  Copyright (C) 2004-2011 Kazuki Tsujimoto, All rights reserved.

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
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "vmw_door.h"

static unsigned char txtbuf[VMW_TEXT_SIZE_MAX];

int main(int argc, char *argv[])
{
    if (vmw_check_env()) {
        ssize_t n;
        int fd;

        fd = (argc == 1) ? 0 : open(argv[1], O_RDONLY);
        if (fd == -1) {
            fprintf(stderr, "open(2) failed: %s\n", strerror(errno));
            return 1;
        }
        n = read(fd, txtbuf, sizeof(txtbuf));
        if (close(fd) == -1) {
            fprintf(stderr, "close(2) failed: %s\n", strerror(errno));
            return 1;
        }

        if (n >= 0) {
            int i;
            ssize_t txtlen;
            unsigned char *p, *tail;

            /* remove CRs from txtbuf */
            for (p = tail = txtbuf; p < txtbuf + n; p++) {
                if (*p != '\r') {
                    *tail++ = *p;
                }
            }
            txtlen = tail - txtbuf;

            vmw_door_t regs = {VMW_ADDR, txtlen, VMW_CMD_SET_TXT_LEN, VMW_PORT};
            vmw_door(&regs);

            for (i = 0; i < txtlen; i += 4) {
                vmw_door_t regs = {VMW_ADDR,
                                   ((i+3 >= txtlen) ? 0 : txtbuf[i+3] << (8*3)) |
                                   ((i+2 >= txtlen) ? 0 : txtbuf[i+2] << (8*2)) |
                                   ((i+1 >= txtlen) ? 0 : txtbuf[i+1] << (8*1)) |
                                   (txtbuf[i]),
                                   VMW_CMD_SET_TXT, VMW_PORT};
                vmw_door(&regs);
            }
        } else {
            return 1;
        }
    } else {
        return 1;
    }

    return 0;
}
