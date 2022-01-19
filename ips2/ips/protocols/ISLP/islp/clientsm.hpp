#pragma once
#include <functional>

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
      explicit clientStatemachine(intercomStation &self, arcCrypt *key);

      /*!
       * \brief operator () proceed an message given as parameter
       * \param message pointer to an message to proceed
       * \return
       */

      ISLP::State operator()(ISLP::basicMessage &message);

      /*!
       * \brief setSenderFunction sets a pointer to the function that called back to send data
       * \note this is C-style function pointer.
       */

      void setSenderFunction(void(*const callbackFcn)(uint8_t *, size_t));
      void setSenderFunction(std::function<void(uint8_t*, size_t)> callback);

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
      std::function<void(uint8_t*, size_t)> *m_callback=nullptr;

      ISLP::State proceed(basicMessage &message);
      virtual ISLP::State yellHandler(ISLP::yellMsg &message);
      virtual ISLP::State setIpHandler(ISLP::setStationMessage &message);
      ISLP::State checkYellValid(yellMsg &message);
      void setKey(uint32_t key);
      int responce();
    };

}
