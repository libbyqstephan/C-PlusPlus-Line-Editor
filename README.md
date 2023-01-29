# C-PlusPlus-Line-Editor
Create, edit, and save a text file all through the command line.

This C++ program is an assignment from an Advanced Data Structures class. It implements linear stuctures (vectors) to create a line editor that will allow users to create or open an existing text file for editing purposes, then save for later using a given set of commands.

To open a file for editing, the editor allows the user to enter a filename as a command line parameter. If that file does not exist, one will be created. If no filename is entered, the program will continue, but the user will have to create one if they wish to "write" (save).

Once activated, all commands to navigate are given with a single character and arguments ([] indicate optional) as follows:
- W [filename] - Writes/saves the contents of the current "buffer" to a file. If no filename was entered as an argument, then it uses the filename opened upon start. If no file was been opened upon start, then the editor indicates to the user that a file name must be given.
- J line_number - Jumps to a line in the file. -1 will jump to the end of the file, and 0 will jump to the beginning.
- I [some text] - Inserts the text at the "current" line. If no text is given, it will insert a blank line.
- A [some text] - Inserts the text after the "current" line. If no text is given, it will insert a blank line
- L [line number a] [line number b] - Lists the contents of the buffer from line a to b (inclusive). If no line numbers are given, it lists the contents of the entire buffer. If one line number is given, shows only that line.
- D [line number a] [line number b] - Deletes the contents of the buffer from line a to b (inclusive), where a < b. If no line numbers are given, it deletes the current line. If 1 line number is given, it deletes that line.
- Q - Quits the editor. If the buffer has been modified and not written/saved, it notifies the user and asks if they want to save changes before quitting.
