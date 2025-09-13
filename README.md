# LoginPage

This readme explains all the changes to the LoginPage files on March 29

## Changes to LoginWindow.cpp

Added <#include "api.h"> to include API functions header file.

Replaced these two functions:
- on_loginButton_clicked(): the login button
- on_retrieveButton_clicked(): the "retrieve password" button in the Forgot Password page

Now they are integrated with the test.cpp API functions login and restore_password.



Also added two new functions: 
- on_signupPageButton_clicked():
  - The "Sign Up" Button on the login page that redirects user to sign up page
- on_signupButton_clicked():
  - The actual Sign Up button inside the Sign Up page that signs up user and switches to main program page if successful

The signup button (signup_Button) is integrated with signup function from the API in test.cpp, and shows error if username is taken already.


## Changes to LoginWindow.h

Added  these 3 headers:

- void on_signupButton_clicked();
- void on_signupPageButton_clicked(); 
- void on_backButton_7_clicked(); - back button on signup page
 
## Changes to LoginWindow.ui
 - added a new page (page 5) that holds the signup page contents

## Changes to LoginPage.pro: 
- added "INCLUDEPATH += $$PWD/Headers" manually at the top
- everything else changed automatically, such as the api.h file in the headers appeared when api.h was created

## New files added:
- api.cpp: the api functions from test.cpp for signup, login, restore password
- api.h: api function headers
- httplib.h: downloaded from cpp-httplib folder in API folder in group51 repo
- ../json.hpp: downloaded from json folder in API folderin group51 repo
- the last 2 files just make the api calls work
