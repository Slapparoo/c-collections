#include "src/objects.h"
#include "src/pointer_list.h"

struct Testing123_code
{
    void (*addAge)(Pointer, int);
    void (*setName)(Pointer, PChar);
    void (*print)(Pointer);
    PChar (*toPChar)(Pointer);
} Testing123_code;

struct Testing123_data
{
    int age;
    PChar name; 
    struct Testing123_code * code;
} Testing123_data;

typedef struct Testing123_data * PTesting123;

void Testing123_addAge(PTesting123 data, int amount) 
{
    data->age += amount;
}

void Testing123_setName(PTesting123 data, PChar name) 
{
    data->name = name;
}

/**
 * @brief acallers are responsible for freeing the char* memory
 * 
 * @param data 
 * @return char* 
 */
PChar Testing123_toPChar(PTesting123 data) 
{
    PChar format = ANSI_WHITE "age " ANSI_BOLD_BLUE  "%i, " ANSI_WHITE "name " ANSI_BOLD_BLUE "%s" ANSI_DEFAULT;
    return PStrings.formatString(format, data->age, data->name);
}


void Testing123_print(PTesting123 data) 
{
    PChar message = data->code->toPChar(data);
    Console.format("%s\n", message);
    Pointers.free(message);
}


struct Testing123_code Testing123 = {
    &Testing123_addAge,
    &Testing123_setName,
    &Testing123_print,
    &Testing123_toPChar,
};





int main(int argc, char const *argv[])
{
    struct Testing123_data testing123 = {20, "bob", &Testing123};
    /* code */

    Testing123.print(&testing123);
    Testing123.addAge(&testing123, 2);
    Testing123.print(&testing123);
    Testing123.setName(&testing123, "dave");
    Testing123.print(&testing123);

    PChar name = ANSI_BOLD_CYAN "Brook " "Smith " ANSI_GREEN "was " "here " ANSI_BOLD_YELLOW "2021" " and " "2020" ANSI_DEFAULT;

    Console.format("%s %i\n", name, PStrings.length(name));
    ErrorConsole.println(name);
    Console.println(name);
    ErrorConsole.format(ANSI_BOLD_RED "[ERROR(formated)]" ANSI_DEFAULT " %s %i\n", name, 53);
    ErrorConsole.error(testing123.name);

    printDebug("%s", "a debug message");
    printError("%s", "a error message");
    printInfo("%s", "a info message");
    printWarn("%s", "a warn message");


    f64 number = PStrings.toF64("2342.2342");
    Console.format("number %f\n", number);

    Pointer b = "brook";

    Console.format("pointer size %i\n", sizeof(Pointer));
    Console.format("u64 size %i\n", sizeof(u64));

    return 0;
}
