#include "../baseobject.h"
#include "assert.h"
#include "ctype.h"

void usage() {
    Console.println("expand the base object template");
    Console.println("<ClassName> [extends]");
    Console.println("Case name eg MyClass, and the class it extends");
}

PChar replaceAll(PChar content, PChar className, PChar extends) {
    PChar upperName = PStrings.toUpper(PStrings.clone(className)), 
          lowerName = PStrings.toLower(PStrings.clone(className)), 
          lowerExtends = PStrings.toLower(PStrings.clone(extends)),
          upperExtends = PStrings.toUpper(PStrings.clone(extends));

    PChar tmp1 = PStrings.replace(content, "$Template$", className);
    PChar tmp2 = PStrings.replace(tmp1, "$TEMPLATE$", upperName);
    Pointers.free(tmp1);
    tmp1 = PStrings.replace(tmp2, "$EXTENDS$", upperExtends);
    Pointers.free(tmp2);
    tmp2 = PStrings.replace(tmp1, "$extends$", lowerExtends);
    Pointers.free(tmp1);
    tmp1 = PStrings.replace(tmp2, "$template$", lowerName);
    Pointers.free(tmp2);

    Pointers.free(upperName);
    Pointers.free(lowerName);
    Pointers.free(lowerExtends);
    Pointers.free(upperExtends);
    
    return tmp1;
}


void writeFile(PChar filename, PChar content) {
    File f =Files.open(filename, "w");
    assert(f);
    Files.printString("// Generated from template $TEMPLATE$.h\n", f);
    Files.printString(content, f);
    Files.flush(f);
    Files.close(f);
}

PChar readFile(PChar filename) {
    File f = Files.open(filename, "r");
    assert(f);
    u64 size = Files.size(f);
    assert(size);
    PChar content = Pointers.malloc(size +1);

    u64 read = Files.readPointer(content, size, 1, f);
    Console.format("size=%d, read=%d\n", size, read);
    assert(read);
    assert(PStrings.length(content) == size);
    Files.close(f);

    return content;
}

int main(int argc, char const *argv[])
{
    Console.format("arg count %d\n", argc);
    if (argc < 2) {
        usage();
        return 1;
    }

    PChar extends;
    if (argc == 3) {
        extends = argv[2];
    } else {
        extends = "BaseObject";
    }

    PChar className = argv[1],
        lowerName = PStrings.toLower(PStrings.clone(className)),
        headerFilename = PStrings.formatString("%s.h", lowerName),
        codeFilename = PStrings.formatString("%s.c", lowerName);
    assert(headerFilename);
    assert(codeFilename);

    PChar content = readFile("$template$.h");
    PChar newContent = replaceAll(content, className, extends);
    writeFile(headerFilename, newContent);
    Pointers.free(content);
    Pointers.free(newContent);

    content = readFile("$template$.c");
    newContent = replaceAll(content, className, extends);
    writeFile(codeFilename, newContent);
    Pointers.free(content);
    Pointers.free(newContent);

    Pointers.free(lowerName);
    Pointers.free(codeFilename);
    Pointers.free(headerFilename);

    return 0;
}
