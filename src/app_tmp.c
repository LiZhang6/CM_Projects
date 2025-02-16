/*
******************************************************************************
**  CarMaker
**  Vehicle Dynamics Simulation Toolkit
**
**  Copyright (C)   IPG Automotive GmbH
**                  Bannwaldallee 60             Phone  +49.721.98520.0
**                  76185 Karlsruhe              Fax    +49.721.98520.99
**                  Germany                      WWW    www.ipg-automotive.com
******************************************************************************
*/

#include <Global.h>

#if defined(WIN32)
#  include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <infoc.h>
#include <CarMaker.h>
#include <ipgdriver.h>
#include <road.h>

extern const char *SetConnectedIO (const char *io);

static const char *CompileLibs[] = {
    /* ../../../fake/linux64/lib-xeno/../../../src_lib/Portings/xeno/lib/libcar.a */
    /* ../../../fake/linux64/lib-xeno/../../../src_lib/Portings/xeno/lib/libcarmaker.a */
    /* ../../../fake/linux64/lib-xeno/../../../lib/driver/xeno/lib/libipgdriver.a */
    /* ../../../fake/linux64/lib-xeno/../../../lib/road/xeno/lib/libipgroad.a */
    /* ../../../fake/linux64/lib-xeno/../../../lib/tire/xeno/lib/libipgtire.a */
    "libcar.a	CarMaker-Car xeno 13.0 2023-11-14",
    "libcarmaker.a	CarMaker xeno 13.0 2023-11-14",
    "libipgdriver.a	IPGDriver xeno64 13.0 2023-11-09",
    "libipgroad.a	IPGRoad linux64 13.0 2023-11-10",
    "libipgtire.a	IPGTire linux64 9.1 2023-03-24",
    NULL
};


static const char *CompileFlags[] = {
    "-m64 -fPIC -O3 -DNDEBUG -DLINUX -DLINUX64 -D_GNU_SOURCE",
    "-D_FILE_OFFSET_BITS=64 -DCM_NUMVER=130000",
    "-D_REENTRANT -fasynchronous-unwind-tables",
    "-D__COBALT__ -DXENO -DXENO64 -DCM_HIL -DMYMODELS -Wall",
    "-Wimplicit -Wmissing-prototypes",
    "-Wno-format-overflow -Wno-format-truncation",
    "-Wno-stringop-overflow -Wno-stringop-truncation",
    "-Wno-array-bounds -Wno-array-parameter",
    "-Wno-attributes -Wno-packed-not-aligned",
    "-Wno-address-of-packed-member -fno-stack-protector",
    "-Wlogical-op -Wno-return-local-addr",
    NULL
};


tAppStartInfo   AppStartInfo = {
    "CarMaker 13.0 - Car_Generic",          /* App_Version         */
    "2",          /* App_BuildVersion    */
    "jetbrains",     /* App_CompileUser     */
    "b1fdf6cc7772",         /* App_CompileSystem   */
    "2023-11-14 16:22:26",  /* App_CompileTime */

    CompileFlags,                /* App_CompileFlags  */
    CompileLibs,                 /* App_Libs          */

    "",          /* SetVersion        */

    NULL,           /* TestRunName       */
    NULL,           /* TestRunFName      */
    NULL,           /* TestRunVariation  */
    NULL,           /* LogFName          */

    0,              /* SaveMode          */
    0,              /* OnErrSaveHist     */

    0,              /* Verbose           */
    0,              /* Comments          */
    0,              /* ModelCheck        */
    0,              /* Snapshot          */
    0,              /* DriverAdaption    */
    0,              /* Log2Screen        */
    0,              /* ShowDataDict      */
    0,              /* DontHandleSignals */
    {0, 0, NULL},   /* Suffixes          */
    {0, 0, NULL}    /* Keys              */
};



void
App_InfoPrint (void)
{
    int i;
    Log ("Application.Version       = %s #%s (%s)\n",
            AppStartInfo.App_Version,
            AppStartInfo.App_BuildVersion,
            SimCoreInfo.Version);
    Log ("Application.Compiled      = %s@%s %s\n",
            AppStartInfo.App_CompileUser,
            AppStartInfo.App_CompileSystem,
            AppStartInfo.App_CompileTime);
    Log ("Application.BuildVersion  = %s\n", AppStartInfo.App_BuildVersion);
    Log ("Application.CompileTime   = %s\n", AppStartInfo.App_CompileTime);
    Log ("Application.CompileUser   = %s\n", AppStartInfo.App_CompileUser);
    Log ("Application.CompileSystem = %s\n", AppStartInfo.App_CompileSystem);

    i = 0;
    Log ("Application.CompileFlags:\n");
    while (AppStartInfo.App_CompileFlags != NULL
        && AppStartInfo.App_CompileFlags[i] != NULL) {
        Log ("			%s\n", AppStartInfo.App_CompileFlags[i++]);
    }

    i = 0;
    Log ("Application.Libs:\n");
    while (AppStartInfo.App_Libs != NULL && AppStartInfo.App_Libs[i] != NULL)
        Log ("			%s\n", AppStartInfo.App_Libs[i++]);

#if 0
    /* Security */
    i = 0;
    Log ("Application.Suffixes:\n");
    while (AppStartInfo.Suffix.List != NULL && AppStartInfo.Suffix.List[i] != NULL)
        Log ("			%s\n", AppStartInfo.Suffix.List[i++]);

    i = 0;
    Log ("Application.Keys:\n");
    while (AppStartInfo.Key.List != NULL && AppStartInfo.Key.List[i] != NULL)
        Log ("			%s\n", AppStartInfo.Key.List[i++]);


    /*** Linked libraries */
    Log ("Application.Version.Driver =\t%s\n",  IPGDrv_LibVersion);
    Log ("Application.Version.Road =\t%s\n",    RoadLibVersion);
#endif
}




int
App_ExportConfig (void)
{
    int        i, n;
    char       *txt[42], sbuf[512];
    char const *item;
    tInfos *inf = SimCore.Config.Inf;

    InfoSetStr (inf, "Application.Version",       AppStartInfo.App_Version);
    InfoSetStr (inf, "Application.BuildVersion",  AppStartInfo.App_BuildVersion);
    InfoSetStr (inf, "Application.CompileTime",   AppStartInfo.App_CompileTime);
    InfoSetStr (inf, "Application.CompileUser",   AppStartInfo.App_CompileUser);
    InfoSetStr (inf, "Application.CompileSystem", AppStartInfo.App_CompileSystem);
    if (AppStartInfo.App_CompileFlags != NULL)
        InfoSetTxt (inf, "Application.CompileFlags",
                    (char**)AppStartInfo.App_CompileFlags);
    InfoAddLineBehind (inf, NULL, "");
    if (AppStartInfo.App_Libs != NULL)
        InfoSetTxt (inf, "Application.Libs",
                    (char**)AppStartInfo.App_Libs);
    InfoAddLineBehind (inf, NULL, "");

    /*** Linked libraries */
    InfoSetStr (inf, "Application.Version.Driver",  IPGDrv_LibVersion);
    InfoSetStr (inf, "Application.Version.Road",    RoadLibVersion);
    InfoAddLineBehind (inf, NULL, "");

    /*** I/O configuration */
    IO_ListNames(sbuf, -1);

    n = 0;
    txt[n] = NULL;
    while (1) {
	item = strtok((n==0 ? sbuf : NULL), " \t");
	if (item == NULL || n >= 42-1)
	    break;

	txt[n++] = strdup(item);
	txt[n] =   NULL;
    }

    InfoSetTxt (inf, "IO.Configs", txt);
    InfoAddLineBehind (inf, NULL, "");

    for (i=0; i < n; i++)
	free (txt[i]);

    return 0;
}



