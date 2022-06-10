/*!
 \file tests.h

*/

#ifndef TESTS_H
#define TESTS_H

class StateTests;

/*!
 \brief This class has some methods for the testing system

 \class Tests tests.h "tests.h"
*/
class Tests
{
    class StateTests *current; /*! current state of the testing system */
public:
    /*!
     \brief Base constructor

     \fn Tests
    */

    Tests();

    /*!
     \brief set up a state for the state machine

     \fn setCurrent
     \param s current state of tests
    */
    void setCurrent(StateTests *s);

    /*!
     \brief test for ITM system

     \fn debugTest
    */
    void debugTest();

    /*!
     \brief rs232 (uart) test for our platform

     \fn rs232Test
    */
    void rs232Test();

    /*!
     \brief test for FatFs, need to check internal flash of mcu

     \fn fatFsTest
    */
    void fatFsTest();

    /*!
     \brief test for littlefs, need to check functions of littlefs

     \fn littleFsTest
    */
    void littleFsTest();

    /*!
     \brief need to check randomization of mcu

     \fn trngTest
    */
    void trngTest();

    /*!
     \brief test for getting UID of mcu

     \fn uidTest
    */
    void uidTest();

    /*!
     \brief this is the finish test

     \fn testsCompleted
    */
    void testsCompleted();
};

/*!
 \brief This class is needed to functions for debugging

 \class StateTests tests.h "tests.h"
*/
class StateTests
{
public:
    /*!
     \brief test for ITM system

     \fn debugTest
     \param m
    */
    virtual void debugTest(Tests *m) {}
    /*!
     \brief rs232 (uart) test for our platform

     \fn rs232Test
     \param m
    */
    virtual void rs232Test(Tests *m) {}
    /*!
     \brief test for FatFs, need to check internal flash of mcu

     \fn fatFsTest
     \param m
    */
    virtual void fatFsTest(Tests *m) {}
    /*!
     \brief test for littlefs, need to check functions of littlefs

     \fn littleFsTest
     \param m
    */
    virtual void littleFsTest(Tests *m) {}
    /*!
     \brief need to check randomization of mcu

     \fn trngTest
     \param m
    */
    virtual void trngTest(Tests *m) {}
    /*!
     \brief test for getting UID of mcu

     \fn uidTest
     \param m
    */
    virtual void uidTest(Tests *m) {}
    /*!
     \brief this is the finish test

     \fn testsCompleted
     \param m
    */
    virtual void testsCompleted(Tests *m) {}
};

/*!
 \brief Test for Debug class

 \class DebugTest tests.h "tests.h"
*/
class DebugTest : public StateTests
{
public:
    /*!
     \brief

     \fn DebugTest
    */
    DebugTest() {}
    /*!
     \brief Destructor

     \fn ~DebugTest
    */
    ~DebugTest() {}
    /*!
     \brief rs232 (uart) test for our platform

     \fn rs232Test
     \param m
    */
    void rs232Test(Tests *m);
};

/*!
 \brief Test for RS232 class

 \class RS232Test tests.h "tests.h"
*/
class RS232Test : public StateTests
{
public:
    /*!
     \brief

     \fn RS232Test
    */
    RS232Test() {}
    /*!
     \brief Destructor

     \fn ~RS232Test
    */
    ~RS232Test() {}
    /*!
     \brief test for FatFs, need to check internal flash of mcu

     \fn fatFsTest
     \param m
    */
    void fatFsTest(Tests *m);
};

/*!
 \brief Test for FatFs class

 \class FatFsTest tests.h "tests.h"
*/
class FatFsTest : public StateTests
{
public:
    /*!
     \brief Constructor

     \fn FatFsTest
    */
    FatFsTest() {}

    /*!
     \brief Destructor

     \fn ~FatFsTest
    */
    ~FatFsTest() {}

    /*!
     \brief test for littlefs, need to check functions of littlefs

     \fn littleFsTest
     \param m
    */
    void littleFsTest(Tests *m);
};

/*!
 \brief Test for littlefs class

 \class LittleFsTest tests.h "tests.h"
*/
class LittleFsTest : public StateTests
{
public:

    /*!
     \brief Constructor

     \fn LittleFsTest
    */
    LittleFsTest() {}

    /*!
     \brief Destructor

     \fn ~LittleFsTest
    */
    ~LittleFsTest() {}

    /*!
     \brief simple test for UID

     \fn uidTest
     \param m
    */
    void uidTest(Tests *m);
};

/*!
 \brief Test for UID test

 \class UidTest tests.h "tests.h"
*/
class UidTest : public StateTests
{
public:

    /*!
     \briefc

     \fn UidTest
    */
    UidTest() {}

    /*!
     \brief Destructor

     \fn ~UidTest
    */
    ~UidTest() {}

    /*!
     \brief need to check randomization of mcu

     \fn trngTest
     \param m
    */
    void trngTest(Tests *m);
};

/*!
 \brief Test for TRNG class

 \class Trng tests.h "tests.h"
*/
class Trng : public StateTests
{
public:

    /*!
     \brief Constructor

     \fn Trng
    */
    Trng() {}

    /*!
     \brief Destructor

     \fn ~Trng
    */
    ~Trng() {}

    /*!
     \brief this is the finish test

     \fn testsCompleted
     \param m
    */
    void testsCompleted(Tests *m);
};

#endif // TESTS_H
