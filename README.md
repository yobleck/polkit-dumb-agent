polkit-dumb-agent-style
-----------------

polkit is dumb, and never works.

so here is probably the simplest version you can have (except the first one
that just ACKed every call), 97 lines of C++ and 45 lines of C, in total 142
lines of code.

it just uses kdesu to call a helper as root to send the reply, so all requests
requires the user to enter the root password.


TODO
----

Write a simple replacement for polkit itself, it's a simple dbus API and
90% of what it provides is never used.

fix cmake to have sane paths


Requirements
------------

 - C++ compiler
 - Qt5 (dbus and widgets)
 - kdesu

Style additions
---------------

Note: these style additions are curently WIP and have weird compile settings

 - -h,--help
   - menu that provides list of available Qt styles
 - -s,--style
   - set style
 
 
Custom Installation
-------------------

 - download from here (sorry no package repository)
 - cd to folder with files in it
 - run: ccmake $PWD (read up on ccmake if you want more than basic instructions)
 - once in ccmake hit the c key
 - set the install dir on the second line
 - hit c again to clear asterisk chars
 - hit g key (ccmake should close)
 - run: make
 - move polkit-dumb-agent-style to exec dir i.e. /usr/local/bin/
 - move polkit-dumb-agent-responder-style to lib dir i.e. /usr/local/lib/polkit-dumb-agent-style/
 - add exec to your autostart script or put the .desktop file in /etc/xdg/autostart (make sure no other polkit agents are running)
