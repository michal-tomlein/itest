# Changelog

## 2.0.0

- Simplified the scoring system (for multiple-choice questions if partially correct answers are allowed)
- Renamed flags to categories
- Removed the session archive
- Removed the database information section

## 1.4.2

- Added Ukrainian, Czech and Hungarian translations
- Fixed the "Print questions" feature
- Added the ability to sort questions by flag

## 1.4.1 (2009-04-04)

- Added Latvian translation
- Number of flags now unlimited
    - iTest 1.4.0 won't be able to open your database if you
      create more than 20 flags (otherwise, the database format
      is unchanged and compatible)
- Added the "Shuffle answers" and "Do not print test results"
  options to server settings
- "Other settings" moved from iTestClient to iTestServer
- Mac OS X: double click or drag and drop database files onto
  the iTest icon to open them
- Upgraded from Qt 4.4.0 to Qt 4.5.0 (Windows and Mac OS X
  binary packages; tarballs still build with Qt 4.4)
- Numerous bug-fixes

## 1.4.0 (2008-07-04)

- Added Italian translation
- Added the ability to create classes with students
- Added an advanced scoring system
- Added the ability to set the (minimum) number of questions in
  a test for each flag separately
- Added multiple choice questions support
- Added support for up to 9 answers and the ability to set
  this number for each question separately
- Advanced test printing options, random order of questions in
  each printout, the ability to print keys to tests
- Improved appearance under Mac OS X
- New database format - older versions of iTest cannot open the
  new itdb 1.4 files, iTest 1.4 can still open older databases
- Code clean-up, code sharing
- Upgraded from Qt 4.3.2 to Qt 4.4.0
- Numerous bug-fixes

## 1.3.0 (2007-11-10)

- Added Spanish translation
- Added SVG (scalable vector graphics) support, which means
  questions can now contain graphics
- Added the ability to print questions and tests (with or
  without graphics)
- Added multiple correct answers support
- Added search to the "overall statistics" dialogue
- Made the UI more flexible
- Added the ability to hide correct answers at the end of tests (iTestClient)
- Fixed a bug where the status of the test progress bar was not
  refreshed after starting a new test (iTestClient)
- Many improvements in translations
- New database format - older versions of iTest cannot open the
  new itdb 1.3 files, iTest 1.3 can still open older databases
- Upgraded from Qt 4.3.1 to Qt 4.3.2
- Numerous bug-fixes

## 1.2.0 (2007-09-09)

- Renamed to iTestServer/iTestClient
- Added Portuguese translation
- Added the ability to set a pass mark for each flag separately
  - This required some changes in the saved sessions view and
    in printing
- Added the ability to group similar questions in order to be
  able to tell the client to choose one question of the group
  at most (useful when knowing the answer for one question
  would suffice to guess the answers for other questions)
- Server setup (advanced): double clicking adds/removes items
- Improved the "overall statistics" dialogue; question names
  now coloured according to their flag
- Improved the "change language" dialogue
- Improved the comments editor (undo/redo)
- Changed flag 1 colour to light green
- Row height now calculated correctly in the "overall
  statistics" dialogue
- Fixed the "adjust difficulty" button - now the icon in the
  list of questions changes when difficulty changed
- Cleaned up the menus, improved their behaviour
- Added the ability to start a new test (iTestClient)
- More advanced test generation (iTestClient)
- Code clean-up
- New database format - older versions of iTest cannot open the
  new itdb 1.2 files, iTest 1.2 can still open older databases
- Upgraded from Qt 4.3.0 to Qt 4.3.1

## 1.1.1 (2007-06-21)

- Added Turkish translation
- If available, translation to the system language loaded by
  default
- NEW ENCODING: UTF-8 - adds support for more languages and
  special characters
  - iTest 1.1.1 can still open old CP 1250 databases from older
    versions of iTest
  - Older versions of iTest cannot open the new UTF-8 databases
    from iTest 1.1.1
- Upgraded from Qt 4.2.2 to Qt 4.3.0
- Printing: support for the & sign

## 1.1.0 (2007-05-29)

- Added Russian translation
- Removed the ability to delete a session
- Added the ability to archive, restore and copy sessions
- Added the ability to save a copy and a backup copy
- Changes made to questions auto-applied when saving
- The selected session now green in the list of saved sessions
- Decreased font size of printed exam results
- Items in the list of students now green if passed and orange
  if failed
- Warning when changing the name of the database or a question,
  occurrences in sessions and archived sessions auto-updated
- Added the ability to hide question names in Test Writer

## 1.0.3 (2007-04-29)

- Fixed opening of a database (had no visible effect unless you
  had more saved sessions than questions)

## 1.0.2 (2007-04-22)

- Fixed version number, now saved as a float
- Fixed incorrect flags behaviour (auto-assigning of flags when none set)

## 1.0.1 (2007-04-20)

- Fixed off-line client import if a custom test name is used
- Test name used when exporting (off-line session; log)
- Fixed segmentation fault when starting a server session
- Fixed incorrect Slovak translations (window title and others)
- Printing: results progress bar colour auto-adjustment
- Fixed save session checkbox behaviour
- Fixed the "yyyy.MM.dd-hh:mm" vs "yyyy.MM.dd-hh.mm" issue
- Made log export possible even from saved sessions
- Fixed segmentation fault when connecting in Test Writer

## 1.0.0 (2007-04-01)

- First release
