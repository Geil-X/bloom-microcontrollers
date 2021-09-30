#ifndef COMMANDS_COMMAND_FACTORY_H
#define COMMANDS_COMMAND_FACTORY_H

#include <Arduino.h>
#include "Command.h"

class CommandFactory {
public:
    /**
     * Process the user input and place the space separated Commands into the
     * `userCommands` array. The `userCommandCount` is the accompanying value that
     * tells you how many items are in the `userCommands` array.
     *
     * @param userInput The user input Commands
     * @returns command The command that can be executed
     */
    Command *parseCommand(const String &userInput);

private:

    // ---- Command Invocation ----
    Command *newStepper();
    static Command *setupStepper();
    Command *home();
    Command *open();
    Command *openInd();
    static Command *stat();
    static Command *softStop();
    static Command *forceStop();
    static Command *reset();
    static Command *toggleDebug();
    Command *threshold();
    Command *speed();
    Command *acceleration();
    static Command *ping();
    static Command *restart();

    // ---- Helper Functions ----

    /**
     * Process the user input and place the space separated Commands into the
     * `userCommands` array. The `userCommandCount` is the accompanying value that
     * tells you how many items are in the `userCommands` array.
     *
     * @param userInput The user input Commands
     */
    void processUserCommand(const String &userInput);

    /**
     * Parse a string of comma separated integers into an array of integers.
     * @param s The string to parse
     * @return
     *     The parsed array of integers which can be used with the FlowerGroup functions
     *     The array of integers contains the number of values within it as the first index.
     *     For example, for the values 1,2,3 the array returned from this function is {3, 1, 2, 3}
     */
    static int *parseNumbers(String &s);

    /**
     * Determine if a given string is a decimal or not
     */
    static bool isDecimal(const String &value);

    // Info for parsing and accessing user Commands
    int MAX_COMMAND_LENGTH = 10;
    String *userCommand = new String[MAX_COMMAND_LENGTH];
    int userCommandCount = 0;

    int *ALL = new int[1]{0};
};

#endif  // COMMANDS_COMMAND_FACTORY_H
