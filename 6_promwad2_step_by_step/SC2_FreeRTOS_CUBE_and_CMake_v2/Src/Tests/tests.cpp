#include "tests.h"
//#include "../EEPROM/fsforeeprom.h"
#include "../Debug/debug.h"
#include "../uid/uid.h"
#include "../flash/flash.h"
#include "../TRNG/trng.h"
#include "../RS232/rs232.h"

void Tests::setCurrent(StateTests *s)
{
    current = s;
}

Tests::Tests()
{
    current = new DebugTest();
}

void Tests::debugTest()
{
    current->debugTest(this);
}

void Tests::rs232Test()
{
    current->rs232Test(this);
}

void Tests::fatFsTest()
{
    current->fatFsTest(this);
}

void Tests::littleFsTest()
{
    current->littleFsTest(this);
}

void Tests::trngTest()
{
    current->trngTest(this);
}

void Tests::uidTest()
{
    current->uidTest(this);
}

void DebugTest::rs232Test(Tests *m)
{
    Debug::getInstance().dbg << __FUNCTION__ <<  __LINE__ << "\n";
    m->setCurrent(new RS232Test);
    delete this;
}

void RS232Test::fatFsTest(Tests *m)
{
    Debug::getInstance().dbg << "RS232 test \n";
    m->setCurrent(new FatFsTest);
    delete this;
}

void FatFsTest::littleFsTest(Tests *m)
{

    Debug::getInstance().dbg << __FUNCTION__ <<  __LINE__ << "\n";
    m->setCurrent(new LittleFsTest);
    delete this;
}

void LittleFsTest::uidTest(Tests *m)
{


//   Debug::getInstance().dbg << __FUNCTION__ <<  __LINE__ << "\n";
//   FsForEeprom::getInstance().test();
//   m->setCurrent(new UidTest);

    delete this;
}

void UidTest::trngTest(Tests *m)
{
    Debug::getInstance().dbg << __FUNCTION__ <<  __LINE__ << "\n";
    UID::getInstance().test();
    m->setCurrent(new Trng);
    delete this;
}

void Trng::testsCompleted(Tests *m)
{
    Debug::getInstance().dbg << __FUNCTION__ <<  __LINE__ << "\n";
    TRNG::getInstance().test();
    delete this;
}
