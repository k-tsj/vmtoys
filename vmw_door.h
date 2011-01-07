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

#ifndef VMW_DOOR_H
#define VMW_DOOR_H

#define VMW_VERS  6
#define VMW_ADDR  ('V' << 24 | 'M' << 16 | 'X' << 8 | 'h')
#define VMW_PORT  ('V' << 8 | 'X')

#define VMW_CMD_SET_TXT_LEN 0x08
#define VMW_CMD_SET_TXT     0x09
#define VMW_CMD_GET_VERSION 0x0a
#define VMW_CMD_GET_TIME    0x17

#define VMW_TEXT_SIZE_MAX 65436 /* Limitation of VMware host */

typedef struct vmw_door_t { int eax, ebx, ecx, edx; } vmw_door_t;

void vmw_door(vmw_door_t *r);
int vmw_check_env(void);

#endif /* VMW_DOOR_H */
