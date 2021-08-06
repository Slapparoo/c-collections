#include "collections.h"

PChar readFile(PChar filename) {
    File f = Files.open(filename, "r");
    if (!f) {
        return null;
    }
    Files.seek(f, 0L, SEEK_END);
    u64 sz = Files.tellPos(f);
    Files.seek(f, 0L, SEEK_SET);

    PChar text = Pointers.malloc(sz);
    Files.readPointer(text, 1, sz, f);

    Files.close(f);

    return text;
}

#define isAlpha(a) (a >= 'A' && a <= 'Z') || (a >= 'a' && a <= 'z')
#define isNumeric(a) (a >= '0' && a <= '9')
#define isAllowSpecial(a) (a == '$' && a == '_')

PValueHashmap templatesMap;
typedef struct TemplateMapEntry {
    PChar key;
    PChar template;
} TemplateMapEntry;

int loadTemplate(PChar filename) {
    PChar text = readFile(filename);

    int i = 0, last = 0, type = 0;

    boolean newLine = false;
    PChar key = 0;
    
    // block consuming parse
    struct TemplateMapEntry entry;
    while (*text) {
        if (isAlpha(*text)) {
            
            entry.key = text;
            while (*text && *text != '\n') {
                text++;
            }
            if (!*text) break;
            if (*text == '\n') *text = 0;
            // printDebug("block=%s %i", entry.key, hash$PChar((Pointer)&entry));
            text++;
        } else if (*text == '#') {
            // printDebug("comment %c", *text);
            text++;
            while (*text && *text != '\n') {
                text++;
            }
            if (!*text) break;
        } else if (*text == ' ') {
            entry.template = text;
            
            while (*text && !(*text == '\n' && (isAlpha(text[1]) || !text[1]))) {
                text++;
            }
            if (!*text) break;
            if (*text == '\n') *text = 0;
            // printDebug("statement=%s", entry.template);
            ValueHashmap_putEntry(templatesMap, &entry);
            text++;
        }
        else {
            // if (*text == '\r') {
            //     printError("carrage return %c", *text);
            // } else if (*text == '\n') {
            //     printError("new line %c", *text);
            // } else {
            //     printError("unkown token %c", *text);
            // }
            text++;
        }
    }
}

typedef PChar(*Fn_expand)(PChar);

void templateTokenHandler(PChar token) {
    if (*token == ':') {
        // action token
    } else {
        // print token
        if (strchr(token, ',')) {
            // has paramaters
        } else {
            // resolve value
            // order resolver args, map, 
        }
    }

}


PChar templateEntryParser(PChar template) {
    // looking for ${...}
    PChar text = template;
    while (*text) {
        if (startsWith(text, "${")) {
            text+=2;
            PChar tokenStart = text;
            while (*text && *text != '}') {
                text++;
            }
            PChar token = PStrings.subString(tokenStart, text - tokenStart);
            printDebug("token %s", token);
        }
        text++;
    }
}


int main(int argc, char const *argv[])
{

    templatesMap = ValueHashmap_create(sizeof(TemplateMapEntry), &hash$PChar, &compare$Pointer);
    PChar asmBell = "/home/brook/source/brooks-object-c/collections/templates/atant_asm.template";

    // printDebug("%s", readFile(asmBell));
    loadTemplate(asmBell);
    // printDebug("%s", readFile("/home/brook/source/brooks-object-c/collections/test/atant_asm.template"));

    TemplateMapEntry entry = {"popf"};

    TemplateMapEntry * result = ValueHashmap_get(templatesMap, &entry);
    if (result) {
        printDebug("found %s %s", result->key, result->template);
        templateEntryParser(result->template);
    } else {
        printError("not found %s", entry.key);
    }

    return 0;
}
