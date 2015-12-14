#The TwIn-84 CLI/X11 Graphing Calculator
##A collaboration between Will Badart (wbadart) and Cat Badart (cbadart).

Submitted to the dropbox of Will Badart.

The extras folder contains a couple example settings files and the shell script we used to help verify our graphing and calculations. You'll see that the setings files are simply comprised of "set" commands. The instructions for these can be found by running "help" in the calculator. They are also displayed when the program first runs.

To use a particular settings file, run "set file [path/to/file.txt]". settings3.txt is pretty cool.

Also, the program will recognize pi and e, so you may use those constants in expressions. Please note that if your first expression of a session is pi or e alone, it will seg-fault. After that, you may enter pi and e alone to see that they hold the correct values.

A final note about expressions: to see all available functions, refer to the evalFunc function in eval.c. Also, our string validating function does not allow implicit multiplication: you must include a * between nums/variables/parenthases, etc.
If you find that the string validator is incorrectly interpreting something you're sure is a valid string, often times adding 0+... or ...+0 to the beginning or end of the expression will fix it.

Rules for expressions:
-no spaces
-no implicit multiplication

The validation function will call you out on both of these and any other fatal errors in your expression syntax.
