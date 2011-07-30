iTest
=====

iTest is a Qt application for easy computerised examination.
iTest consists of two programs:

* **iTestServer** -- question/answer database editor and test server,
* **iTestClient** -- test client used by students, connects to the server.

iTest makes it easy to:

* Create and organise a database of questions and answers,
* Set up a server and a printer,
* Connect a client computer for each student,
  which generates a test according to your server settings.

Supported platforms:

* Mac OS X
* Linux and various UNIX-based platforms
* Windows

Available in:

* English
* Italian (thanks to *Francesco Atzeni*)
* Latvian (thanks to *Valdis Orols*)
* Portuguese (thanks to *Eugéne Suter*)
* Russian (thanks to *Constantin Makshin* and *Eugene Burmistrov*)
* Slovak
* Spanish (thanks to *Gustavo A. Díaz*)
* Turkish (thanks to *Serdar Soytetir*)

See [screenshots, screencasts][1] and the [documentation][2] for further
information.

[1]: http://itest.sourceforge.net/screenshots.shtml
[2]: http://itest.sourceforge.net/documentation/index.shtml

Installation
============

Binaries are available for **Mac OS X** and **Windows** at the
[downloads page][3].

[3]: http://sourceforge.net/projects/itest/

**Ubuntu** packages are available [here][4] (thanks to Vladimir Yakovlev).
To install, run:

	sudo deb http://ppa.launchpad.net/nagos/ppa/ubuntu karmic main
	sudo apt-get install itest

[4]: https://launchpad.net/~nagos/+archive/ppa/+packages

Building
--------

### Requirements

To build iTest from source, you will need Qt 4.4 or a newer,
compatible version.

You have two options:

1. Compile manually
2. Use the build script to compile iTest

### Compile manually

Regardless of your platform, use the following commands (it is assumed
that you have already changed directory to that of the iTest source code):

	cd iTestServer
	lrelease iTestServer.pro
	cd ..
	cd iTestClient
	lrelease iTestClient.pro
	cd ..

If you're running Mac OS X, type:

	qmake -spec macx-g++ -config release

Otherwise, the following will do:

	qmake -config release

And finally, compile iTest by typing:

	make

(You can also use any other maketool, such as `mingw32-make`, `gmake`
or `nmake`, depending on your platform.)

### Compile using the build script

Download and install [Squirrel Shell][5].

[5]: http://squirrelsh.sourceforge.net/downloads.html

#### Windows

Make sure you have the Qt `bin` directory (usually `C:\Qt\4.x.y\bin`)
in your `PATH` environment variable.
If that is so, you can compile iTest by double-clicking the `build.nut` script.

#### UNIX-based systems

Change directory to that of the iTest source code and type:

	./build.nut

The compiled binaries will be put into the `bin` folder.
