/*!
 \file updatemechanism.h
 \authors Shulenkov R.A.

*/

#ifndef UPDATEMECHANISM_H
#define UPDATEMECHANISM_H

#include <vector>
#include <cstdio>
#include "../Debug/debug.h"

/*!
 \brief It is common object for setting parameters our system

 \class StateInfoBoardData updatemechanism.h "updatemechanism.h"
*/
struct StateInfoBoardData {
    uint32_t id; /*!< TODO: describe, future */
};

/*!
 \brief This class is a mechanism for updating

 \class UpdateMechanism updatemechanism.h "updatemechanism.h"
*/
class UpdateMechanism
{
    std::vector <class ObserverForUpdateMechanism *> views; /*! it is saved classes with the update methods*/
    StateInfoBoardData info; /*! This is the current structure */
public:
    /*!
     \brief Attach a new method

     \fn attach
     \param obs attach ObserverForUpdateMechanish
    */
    void attach(ObserverForUpdateMechanism *obs)
    {
        Debug::getInstance().dbg << __FUNCTION__ <<  __LINE__ << "\n";
        views.push_back(obs);
    }

    /*!
     \brief it sets up a structure for updating

     \fn setInfoData
     \param val setup StateInfoBoardData
    */
    void setInfoData(StateInfoBoardData &val)
    {
        Debug::getInstance().dbg << __FUNCTION__ <<  __LINE__ << "\n";
        info = val;
        notify(); //notify, because data was updated
    }

    /*!
     \brief it is a method for getting information from the structure

     \fn getInfoData
     \return StateInfoBoardData get current state of the structure
    */
    StateInfoBoardData &getInfoData()
    {
        Debug::getInstance().dbg << __FUNCTION__ <<  __LINE__ << "\n";
        return info;
    }

    /*!
     \brief This method is needed to notify all class, which was attached

     \fn notify
    */
    void notify();
};

/*!
 \brief Observer for the update mechanism

 \class ObserverForUpdateMechanish updatemechanism.h "updatemechanism.h"
*/
class ObserverForUpdateMechanism
{
    UpdateMechanism *model; /*!< TODO: describe */
    StateInfoBoardData demon; /*!< TODO: describe */
public:
    /*!
     \brief

     \fn ObserverForUpdateMechanish
     \param mod
     \param diff
    */
    ObserverForUpdateMechanism(UpdateMechanism *mod, const StateInfoBoardData &diff)
    {
        model = mod;
        model->attach(this);
        demon = diff;
        Debug::getInstance().dbg << __FUNCTION__ <<  __LINE__ << "\n";
    }
    /*!
     \brief

     \fn update
    */
    virtual void update() = 0;
protected:

    /*!
     \brief

     \fn getSubject
     \return UpdateMechanism
    */
    UpdateMechanism *getSubject()
    {
        Debug::getInstance().dbg << __FUNCTION__ <<  __LINE__ << "\n";
        return model;
    }

    /*!
     \brief

     \fn getDiff
     \return StateInfoBoardData
    */
    StateInfoBoardData& getDiff(){
        Debug::getInstance().dbg << __FUNCTION__ <<  __LINE__ << "\n";
        return demon;
    }
};

/*!
 \brief

 \fn UpdateMechanism::notify
*/
void UpdateMechanism::notify()
{
    for (int i = 0; i < views.size(); i++) {
        views[i]->update();
        Debug::getInstance().dbg << __FUNCTION__ <<  __LINE__ << "\n";
    }
}

/*!
 \brief This a test class

 \class FirstTestForObserver updatemechanism.h "updatemechanism.h"
*/
class FirstTestForObserver: public ObserverForUpdateMechanism
{
public:

    /*!
     \brief This a test class

     \fn FirstTestForObserver
     \param mod
     \param diff
    */
    FirstTestForObserver(UpdateMechanism *mod, const StateInfoBoardData &diff): ObserverForUpdateMechanism(mod, diff) {}

    /*!
     \brief update method

     \fn update
    */
    void update()
    {
        Debug::getInstance().dbg << __FUNCTION__ <<  __LINE__ << "\n";
    }
};

/*!
 \brief c

 \class SecondTestForObserver updatemechanism.h "updatemechanism.h"
*/
class SecondTestForObserver: public ObserverForUpdateMechanism
{
public:

    /*!
     \brief constructor

     \fn SecondTestForObserver
     \param mod UpdateMechanism
     \param diff StateInfoBoardData
    */
    SecondTestForObserver(UpdateMechanism *mod, const StateInfoBoardData &diff): ObserverForUpdateMechanism(mod, diff) {}

    /*!
     \brief update method

     \fn update
    */
    void update()
    {
        Debug::getInstance().dbg << __FUNCTION__ <<  __LINE__ << "\n";
    }
};

#endif // UPDATEMECHANISM_H
