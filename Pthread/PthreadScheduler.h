/*BEGIN_LEGAL 
#BEGIN_LEGAL
##BEGIN_LEGAL
##INTEL CONFIDENTIAL
##Copyright 2002-2005 Intel Corporation All Rights Reserved.
##
##The source code contained or described herein and all documents
##related to the source code (Material) are owned by Intel Corporation
##or its suppliers or licensors. Title to the Material remains with
##Intel Corporation or its suppliers and licensors. The Material may
##contain trade secrets and proprietary and confidential information of
##Intel Corporation and its suppliers and licensors, and is protected by
##worldwide copyright and trade secret laws and treaty provisions. No
##part of the Material may be used, copied, reproduced, modified,
##published, uploaded, posted, transmitted, distributed, or disclosed in
##any way without Intels prior express written permission.  No license
##under any patent, copyright, trade secret or other intellectual
##property right is granted to or conferred upon you by disclosure or
##delivery of the Materials, either expressly, by implication,
##inducement, estoppel or otherwise. Any license under such intellectual
##property rights must be express and approved by Intel in writing.
##
##Unless otherwise agreed by Intel in writing, you may not remove or
##alter this notice or any other notice embedded in Materials by Intel
##or Intels suppliers or licensors in any way.
##END_LEGAL
#INTEL CONFIDENTIAL
#Copyright 2002-2005 Intel Corporation All Rights Reserved.
#
#The source code contained or described herein and all documents
#related to the source code (Material) are owned by Intel Corporation
#or its suppliers or licensors. Title to the Material remains with
#Intel Corporation or its suppliers and licensors. The Material may
#contain trade secrets and proprietary and confidential information of
#Intel Corporation and its suppliers and licensors, and is protected by
#worldwide copyright and trade secret laws and treaty provisions. No
#part of the Material may be used, copied, reproduced, modified,
#published, uploaded, posted, transmitted, distributed, or disclosed in
#any way without Intels prior express written permission.  No license
#under any patent, copyright, trade secret or other intellectual
#property right is granted to or conferred upon you by disclosure or
#delivery of the Materials, either expressly, by implication,
#inducement, estoppel or otherwise. Any license under such intellectual
#property rights must be express and approved by Intel in writing.
#
#Unless otherwise agreed by Intel in writing, you may not remove or
#alter this notice or any other notice embedded in Materials by Intel
#or Intels suppliers or licensors in any way.
#END_LEGAL
Intel Open Source License 

Copyright (c) 2002-2005 Intel Corporation 
All rights reserved. 
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.  Redistributions
in binary form must reproduce the above copyright notice, this list of
conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.  Neither the name of
the Intel Corporation nor the names of its contributors may be used to
endorse or promote products derived from this software without
specific prior written permission.
 
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE INTEL OR
ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
END_LEGAL */
/* --------------------------------------------------------------------------- */
/* PthreadScheduler:                                                           */
/* schedules the next thread to run                                            */
/* provides the context switching mechanism                                    */
/* --------------------------------------------------------------------------- */

#ifndef PTHREAD_SCHEDULER_H
#define PTHREAD_SCHEDULER_H

#include "PthreadUtil.h"

namespace PinPthread 
{

    class Pthread
    {
      public:
        Pthread(pthread_attr_t*, CONTEXT*, ADDRINT, ADDRINT);
        ~Pthread();
      public:
        bool active;           // whether the thread can be scheduled to run
        bool started;          // whether the thread has begun execution
        ADDRINT* stack;        // thread-specific stack space
        ADDRINT stacksize;     // size of the stack
        CHECKPOINT chkpt;      // checkpointed state before this thread is swapped out
        CONTEXT startctxt;     // context to start this thread
    };

    typedef std::map<pthread_t, Pthread*> pthread_queue_t;

    class PthreadScheduler
    {
      public:
        PthreadScheduler();
        ~PthreadScheduler();
      public:
        void AddThread(pthread_t, pthread_attr_t*, CONTEXT*, ADDRINT, ADDRINT);
        void KillThread(pthread_t);
        void SwitchThreads(CHECKPOINT*, bool);
        void BlockThread(pthread_t);
        void UnblockThread(pthread_t);
        pthread_t GetCurrentThread();
        bool IsThreadValid(pthread_t);
        void GetAttr(pthread_t, pthread_attr_t*);
        UINT32 GetNumActiveThreads();
     private:
        void SetCurrentPtr(pthread_t);
        inline void AdvanceCurrentPtr();
        inline void FixupCurrentPtr();
     private:
        inline pthread_queue_t::iterator GetThreadPtr(pthread_t);
        inline Pthread* GetThread(pthread_queue_t::iterator);
        inline bool IsActive(pthread_t);
        inline bool IsActive(pthread_queue_t::iterator);
        inline void SetActiveState(pthread_t, bool);
        inline bool HasStarted(pthread_queue_t::iterator);
        inline void StartThread(pthread_queue_t::iterator);
        inline CHECKPOINT* GetCurrentCheckpoint();
        inline CONTEXT* GetCurrentStartCtxt();
      private:
        pthread_queue_t pthreads;          // list of thread state
        pthread_queue_t::iterator current; // pointer to the current thread running
        UINT32 nactive;                    // number of active threads
    };
    
} // namespace PinPthread

#endif  // #ifndef PTHREAD_SCHEDULER_H

