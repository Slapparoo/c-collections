#include "assert.h"
#include "collection_base.h"

int main(int argc, char const *argv[])
{
    /* code */

    boolean a = true;
    boolean b = false;
    assert(a);
    assert(!b);
    copy$Boolean(&b, &a);
    assert(a);
    assert(b);
    b = false;
    assert(a);
    assert(!b);
    assert(&a != &b);

    printPrimativeTypeByNum(eBoolean);

    f32 fa = 32.32;
    f32 fb = 77.71;
    assert(fa);
    assert(fb);
    copy$F32(&fa, &fb);
    assert(fa);
    assert(fb);
    assert(fa == fb);
    assert(&fa != &fb);

    printPrimativeTypeByNum(eF32);

    f64 ffa = 32.32;
    f64 ffb = 77.71;
    assert(ffa);
    assert(ffb);
    copy$F64(&ffa, &ffb);
    assert(ffa);
    assert(ffb);
    assert(ffa == ffb);
    assert(&ffa != &ffb);

    PrimativeType* ff64 = &primativeTypes[eF64];
    assert(ff64);
    printPrimativeType(ff64);

    assert(ff64->getset);

    ffa = 32.32;
    assert(ffa != ffb);
    ff64->getset(&ffa, &ffb);
    assert(ffa);
    assert(ffb);
    assert(ffa == ffb);
    assert(&ffa != &ffb);

    return 0;
}

