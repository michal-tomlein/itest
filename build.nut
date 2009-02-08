#!/usr/bin/squirrelsh

local qmakeOpts = [ "-config", "release" ]
local makeTool = "make";

printl("Entering directory: iTestServer");
chdir("iTestServer");

printl("Running QMAKE");
run("qmake", qmakeOpts);

printl("Running LRELEASE (translations)");
run("lrelease", [ "iTestServer.pro" ]);

printl("Leaving directory: iTestServer");
printl("Entering directory: iTestClient");
chdir("../iTestClient");

printl("Running QMAKE");
run("qmake", qmakeOpts);

printl("Running LRELEASE (translations)");
run("lrelease", [ "iTestClient.pro" ]);

printl("Leaving directory: iTestClient");
chdir("..");

printl("Running QMAKE");
run("qmake", qmakeOpts);

switch (getenv("QMAKESPEC"))
{
case "win32-g++":
	makeTool = "mingw32-make";
	break;

case "win32-msvc":
case "win32-msvc.net":
case "win32-msvc2005":
	makeTool = "nmake";
}

printl("Running " + makeTool + " (compiling)");
/* printl("If you use a different maketool");
printl("and do not have MAKE installed,");
printl("you may run it now."); */
run(makeTool);
