/*BEGIN_LEGAL 
INTEL CONFIDENTIAL
Copyright 2002-2005 Intel Corporation All Rights Reserved.

The source code contained or described herein and all documents
related to the source code (Material) are owned by Intel Corporation
or its suppliers or licensors. Title to the Material remains with
Intel Corporation or its suppliers and licensors. The Material may
contain trade secrets and proprietary and confidential information of
Intel Corporation and its suppliers and licensors, and is protected by
worldwide copyright and trade secret laws and treaty provisions. No
part of the Material may be used, copied, reproduced, modified,
published, uploaded, posted, transmitted, distributed, or disclosed in
any way without Intels prior express written permission.  No license
under any patent, copyright, trade secret or other intellectual
property right is granted to or conferred upon you by disclosure or
delivery of the Materials, either expressly, by implication,
inducement, estoppel or otherwise. Any license under such intellectual
property rights must be express and approved by Intel in writing.

Unless otherwise agreed by Intel in writing, you may not remove or
alter this notice or any other notice embedded in Materials by Intel
or Intels suppliers or licensors in any way.
END_LEGAL */
/*
  Test calling an application function from instrumentation.  At the entry
  point to main, we insert a call to an analysis function that calls main
  natively.  The output should show that main was called twice
*/
#include <iostream>
#include "pin.H"

using namespace std;

#if defined(TARGET_MAC)
#define MAINNAME "_main"
#else
#define MAINNAME "main"
#endif

AFUNPTR mainFun = 0;

VOID callmain(INT32 code, VOID *)
{
    mainFun();
}

VOID ImageLoad(IMG img, VOID * v)
{
    RTN mainRtn = RTN_FindByName(img, MAINNAME);
    if (RTN_Valid(mainRtn))
    {
        mainFun = RTN_Funptr(mainRtn);

        RTN_Open(mainRtn);
        RTN_InsertCall(mainRtn, IPOINT_BEFORE, AFUNPTR(callmain), IARG_END);
        RTN_Close(mainRtn);
    }
}

int main(INT32 argc, CHAR **argv)
{
    PIN_InitSymbols();
    PIN_Init(argc, argv);
    
    IMG_AddInstrumentFunction(ImageLoad, 0);
    
    // Never returns
    PIN_StartProgram();
    
    return 0;
}
