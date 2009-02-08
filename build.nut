#!/usr/bin/squirrelsh

local qmakeOpts = [ "-config", "release" ]
local makeTool = "make";

printl("Entering directory: database_editor");
chdir("database_editor");

printl("Running QMAKE");
run("qmake", qmakeOpts);

printl("Running LRELEASE (translations)");
run("lrelease", [ "iTest.pro" ]);

printl("Leaving directory: database_editor");
printl("Entering directory: test_writer");
chdir("../test_writer");

printl("Running QMAKE");
run("qmake", qmakeOpts);

printl("Running LRELEASE (translations)");
run("lrelease", [ "iTestWri.pro" ]);

printl("Leaving directory: test_writer");
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
