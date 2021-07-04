// *Case sensitive replace*
// replace $TEMPLATE$ with your name All uppers
// replace $Template$ with your name First upper cammel case

#include "template.h"

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
void $Template$_init($Template$_data *this) {
    this->classMethods = &$Template$; //
    this->instanceName = $Template$_instance_name;
}

$Template$_data * $Template$_create() {
    $Template$_data *this = Pointers.malloc(sizeof($Template$_data)); 
    $Template$_init(this);
    return this;
}

void $Template$_freeChildren(const $Template$_data * this) {
    this->classMethods->parentClass->freeChildren(this);
}

void $Template$_free($Template$_data ** this) {
    $Template$_freeChildren(*this);
    (*this)->classMethods->parentClass->free(this);
}

int $Template$_hashCode(const $Template$_data * this) {
    return BaseObject.hashCode(this);
}

bool $Template$_equals(const Object this, const Object that) {
    return BaseObject.equals(this, that);
}
    
PChar $Template$_toString($Template$_data * this) {
    return BaseObject.toString(this);
}

struct $Template$_code $Template$ = {
    &BaseObject,
    &$Template$_classname,
    &$Template$_hashCode,
    &$Template$_equals,
    &$Template$_toString,
    &$Template$_free,
    &$Template$_freeChildren,

    &$Template$_create,
    &$Template$_init
};

// #define TEST_$TEMPLATE$
#ifdef TEST_TEMPLATE

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
    assert($Template$.toString);

    $Template$_data * template = $Template$.create();
    assert(template);
    assert(template->classMethods);
    assert(template->instanceName);
    assert(template->privateData == null);
    assert(template->toStringPString == null);
    assert(template->classMethods->parentClass);

    printf("instanceName %s\n", template->instanceName);
    printf("className %s\n", template->classMethods->className());

    return 0;
}
#endif
