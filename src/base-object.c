#include "base-object.h"


PChar BaseObject_classname() {
    return BaseObject_name;
}

/**
 * @brief initialise the required values this may be called directly 
 * where memory has been allocated from something else like a large block of memory
 * if memory is not "controlled by this object, then call freeChildren as opposed to free
 * 
 * @param this 
 */
void BaseObject_init(Object this) {
    this->classMethods = &BaseObject; //
    this->instanceName = BaseObject_instance_name;
}

/**
 * @brief Probably whould never get used but here for completeness
 * 
 * @return BaseObject_data* 
 */
Object  BaseObject_create() {
    Object this = Pointers.malloc(sizeof(BaseObject_data)); 
    BaseObject_init(this);
    return this;
}

void BaseObject_freeChildren(const Object  this) {
    
}

i32 BaseObject_hashCode(const Object  this)
{
    return Hashes.hash64((i64)this);
}

boolean BaseObject_equals(const Object  this, const Object  that) 
{
    return this == that;
}
  
Pointer BaseObject_toString(Object  this)
{
    if (this->toStringPString) {
        free(this->toStringPString);
    } 

    this->toStringPString = PStrings.formatString("%s @%p", this->instanceName, this);

    return this->toStringPString;
}

void BaseObject_free(Object * this) 
{
    // ** free the toStringString
    if ((*this)->toStringPString) {
        free((*this)->toStringPString);
    } 


    free((*this));
    *this = 0;
}  

BaseObject_code BaseObject = {
    null,
    &BaseObject_classname,
    &BaseObject_hashCode,
    &BaseObject_equals,
    &BaseObject_toString,
    &BaseObject_free,
    &BaseObject_freeChildren,
};

#ifdef TEST_BASEOBJECT

#include <assert.h>

int main(int argc, char const *argv[])
{

    assert(BaseObject.className);
    assert(BaseObject.equals);
    assert(BaseObject.free);
    assert(BaseObject.freeChildren);
    assert(BaseObject.hashCode);
    assert(BaseObject.parentClass == null);
    assert(BaseObject.toString);

    Object object = BaseObject_create();
    assert(object);

    assert(object->classMethods);
    assert(object->instanceName);
    assert(object->privateData == null);
    assert(object->toStringPString == null);
    assert(object->classMethods->parentClass == null);

    printf("instanceName %s\n", object->instanceName);
    printf("className %s\n", object->classMethods->className());

    return 0;
}


#endif