#pragma once

#include "details.hpp"

namespace ISLP {

  /**
   * @brief The clientSm class - ISLP client state machine
   */

  class clientStatemachine
    {
    public:
      clientStatemachine() = delete;
      clientStatemachine(const clientStatemachine &other) = delete;
      clientStatemachine(clientStatemachine &&other) = delete;
      /*!
       * \brief clientSm The only constructor!!!
       * \param self Pointer to a self intercomStation structure (must been created!)
       * \param key Pointer to a cipher object to en/decode msgs. In case of nullptr object will be created dynamicly.
       */
      explicit clientStatemachine(intercomStation *self, arcCrypt *key = nullptr);

      /*!
       * \brief operator () proceed an message given as parameter
       * \param message pointer to an message to proceed
       * \return
       */

      ISLP::State operator()(ISLP::basicMessage *message);

      /*!
       * \brief setSenderFunction sets a pointer to the function that called back to send data
       * \note this is C-style function primitive.
       * \todo rewrite/add std C++ funcktions invokation(std::function likee).
       */

      void setSenderFunction(void (*callbackFcn)(uint8_t*, size_t));

      /*!
       * \brief getState returns last state
       * \return
       */

      ISLP::State getState();

      /*!
       * \brief getKey returns current cipher object pointer
       * \return
       */

      arcCrypt* getKey();
    protected:

      intercomStation *m_self = nullptr; //! Pointer to self intercomStation structure.
      arcCrypt *m_key = nullptr; //! Pointer to Crypt object (automaticly created in case of nullptr(default)).
      ISLP::State m_state = ISLP::State::None; //! Current state (after last operation).

      void (*senderFcn)(uint8_t*, size_t)=nullptr; //! C-style callback function that sends data to the interface.

      ISLP::State proceed(ISLP::basicMessage *message);
      virtual ISLP::State yellHandler(ISLP::yellMsg *message);
      virtual ISLP::State setIpHandler(ISLP::setStationMessage *message);
      ISLP::State checkYellValid(ISLP::yellMsg *message);
      void setKey(uint32_t key);
      int responce();
    };

}
