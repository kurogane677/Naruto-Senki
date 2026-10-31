// Win32 crash handler for development builds: on an unhandled exception it
// writes a symbolized stack trace (function, file, line from the PDB) to
// crash.log next to the executable and a minidump to crash.dmp.
#pragma once

void installCrashHandler();
