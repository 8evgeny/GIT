#ifndef IRQCALLBACK_H_
#define IRQCALLBACK_H_

#include <constants.h>
#include "PeripheralUnit.h"
#include <vector>
#include <memory>

typedef void (*SimpleCallback)(void);

class IrqCallbackBase
{
  public:
    IrqCallbackBase(SimpleCallback function){ callback = function; }
    static void staticInvoke(uint8_t index){ irqPeripherals[index]->interruptHandler(); }
    static std::vector<PeripheralUnit*> irqPeripherals;

    operator SimpleCallback() const { return callback; }

  private:
    SimpleCallback callback;
};

template<uint8_t I> class IrqDynamicCallback: public IrqCallbackBase
{
  public:
    IrqDynamicCallback() : IrqCallbackBase(&IrqDynamicCallback<I>::generatedStaticFunction){}
    static void generatedStaticFunction(){ return staticInvoke(I); }
};

template<uint8_t I> struct IrqDynamicCallbackFactory
{
  static inline std::shared_ptr<IrqCallbackBase> create(uint8_t index)
  {
    if(index == I)
    {
      return std::shared_ptr<IrqCallbackBase>(new IrqDynamicCallback<I>());
    }
    else
    {
      return IrqDynamicCallbackFactory<I + 1>::create(index);
    }
  }
};

struct Overflow
{
    static inline std::shared_ptr<IrqCallbackBase> create(uint8_t index)
    {
      return NULL;
    }
};

template<> struct IrqDynamicCallbackFactory<constants::isrVectorTableSize> : Overflow{};

std::shared_ptr<IrqCallbackBase> createCallback(uint8_t index)
{
  return IrqDynamicCallbackFactory<0>::create(index);
}

#endif /* IRQCALLBACK_H_ */
