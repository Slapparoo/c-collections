#include "$template$.h"

PChar $Template$_classname() {
    return $Template$_name;
}

/**
 * @brief initialise the required values this may be called directly 
 * where memory has been allocated from something else like a large block of memory
 * if memory is not "controlled by this object, then call freeChildren as opposed to free
 * 
 * @param this 
 */
void $Template$_init(P$Template$ this) {
    this->classMethods = &$Template$; //
    // this->instanceName = $Template$_instance_name;
}

P$Template$ $Template$_create() {
    $Template$_data *this = Pointers.malloc(sizeof($Template$_data)); 
    $Template$_init(this);
    return this;
}

void $Template$_freeChildren(const P$Template$ this) {
    this->classMethods->parentClass->freeChildren(this);
}

void $Template$_free(P$Template$ * this) {
    $Template$_freeChildren(*this);
    (*this)->classMethods->parentClass->free(this);
}

i64 $Template$_hashCode(const P$Template$ this) {
    return BaseObject.hashCode(this);
}

bool $Template$_equals(const P$Template$ this, const P$Template$ that) {
    return BaseObject.equals(this, that);
}
    
PChar $Template$_toPChar(P$Template$ this) {
    return BaseObject.toPChar(this);
}

Pointer $Template$_$introspectorByI64(i64 definition) {
    switch(definition) {
        case 6490546018889844307:
            // Pointer(*$introspectorByPChar)(PChar)
            return $Template$.$introspectorByPChar;
        case 2914503459602455398:
            // Pointer(*$introspectorByI64)(i64)
            return $Template$.$introspectorByI64;
        case 452410809924657724:
            // PChar(*className)()
            return $Template$.className;
        case 4344075123911651329:
            // i32(*hashCode)(Object)
            return $Template$.hashCode;
        case 3272267969087222019:
            // boolean(*equals)(Object,Object)
            return $Template$.equals;
        case 6630242114221533020:
            // PChar(*toPChar)(Object)
            return $Template$.toPChar;
        case 1660781971090967059:
            // void(*free)(Object)
            return $Template$.free;
        case 7307130038560973942:
            // void(*freeChildren)(Object)
            return $Template$.freeChildren;
        case 1545141691094564187:
            // MyClass_data*(*create)()
            return $Template$.create;
        case 7160215437764159575:
            // void(*init)(MyClass_data)
            return $Template$.init;            
        default:
            return null;
    }
}

Pointer $Template$_$introspectorByPChar(PChar definition) {
    return $Template$_$introspectorByI64(pCharToI64(definition));
}

boolean $Template$_$implements(i64 definition) {
    // switch (definition) case _ID_OF_INTERFACE_ return true
    return false;
}

// PChar[] interfaces = {"Comparable", "Runnable", "Closable"}

PChar* $Template$_$implemented() {
    // return interfaces
    return null;
}


struct $Template$_code $Template$ = {
    &BaseObject,
    &$Template$_$introspectorByPChar,
    &$Template$_$introspectorByI64,
    &$Template$_$implements,
    &$Template$_$implemented,
    &$Template$_classname,
    &$Template$_hashCode,
    &$Template$_equals,
    &$Template$_toPChar,
    &$Template$_free,
    &$Template$_freeChildren,

    &$Template$_create,
    &$Template$_init
    // -- add additional functions here
};


// #define TEST_$TEMPLATE$
#ifdef TEST_$TEMPLATE$

#include <assert.h>


int main(int argc, char const *argv[])
{
    assert($Template$.create);
    assert($Template$.className);
    assert($Template$.equals);
    assert($Template$.free);
    assert($Template$.freeChildren);
    assert($Template$.hashCode);
    assert($Template$.init);
    assert($Template$.parentClass);
    assert($Template$.toPChar);

    $Template$_data * $template$ = $Template$.create();
    assert($template$);
    assert($template$->classMethods);
    assert($template$->instanceName);
    assert($template$->$privateData == null);
    assert($template$->$toPChar == null);
    assert($template$->classMethods->parentClass);

    assert($template$->classMethods->className == $Template$.className);
    assert($template$->classMethods->$introspectorByPChar == $Template$.$introspectorByPChar);
    assert($template$->classMethods->$introspectorByI64 == $Template$.$introspectorByI64);
    assert($template$->classMethods->$introspectorByPChar == $Template$_$introspectorByPChar);
    assert($template$->classMethods->$introspectorByI64 == $Template$_$introspectorByI64);

    printf("instanceName %s\n", $template$->instanceName);
    printf("className %s\n", $template$->classMethods->className());
    printf("toPChar %s\n", $template$->classMethods->toPChar($template$));
    
    PChar(*classNamePChar)() = $template$->classMethods->$introspectorByPChar("PChar(*className)()");
    assert(classNamePChar);
    printf("classNamePChar %s\n", classNamePChar());

    PChar(*classNameI64)() = $template$->classMethods->$introspectorByI64(452410809924657724);
    assert(classNameI64);
    printf("classNameI64 %s\n", classNameI64());

    $template$->classMethods->free(&$template$);
    assert(!$template$);

    return 0;
}
#endif
