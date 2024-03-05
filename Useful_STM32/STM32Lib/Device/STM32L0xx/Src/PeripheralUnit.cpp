#include "PeripheralUnit.h"
#include "IrqCallback.h"

constexpr int isrVectorTableOffset = 16;

uint32_t *PeripheralUnit::isrVectorTable;
std::vector<PeripheralUnit*> IrqCallbackBase::irqPeripherals;

PeripheralUnit::PeripheralUnit() : isInitialized(false), interruptFlag(false){}

PeripheralUnit::~PeripheralUnit(){}

void PeripheralUnit::setIsrVectorTable(uint32_t *table)
{
  PeripheralUnit::isrVectorTable = table;
}

bool PeripheralUnit::getInterruptFlag()
{
  return interruptFlag;
}

void PeripheralUnit::clearInterruptFlag()
{
  interruptFlag = false;
}

void PeripheralUnit::init()
{
  isInitialized = true;
}

void PeripheralUnit::initIsr(IRQn_Type irq)
{
  IrqCallbackBase::irqPeripherals.push_back(this);

  isrVectorTable[isrVectorTableOffset + irq] = (uint32_t) ((SimpleCallback) (*createCallback(
      IrqCallbackBase::irqPeripherals.size() - 1)));
}

void PeripheralUnit::interruptHandler()
{
  interruptFlag = true;
}

