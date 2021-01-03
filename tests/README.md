## Irrlicht-ng test suite.

This is composed of a series of tests which exercise basic Irrlicht-ng
functionality.  These are not strictly unit tests, since there is no stub
framework that isolates each method under test.  They do however test small
units of functionality and should help to isolate problems and spot
regressions.

You are encouraged to run the tests whenever you make any significant code
change, and to add tests for any new or modified area of code.

The overall test application will return a count of the number of tests that
failed, i.e. 0 is success.

