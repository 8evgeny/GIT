#ifndef PERIPHERALUNIT_H_
#define PERIPHERALUNIT_H_

#include "STM32.h"

class PeripheralUnit
{
public:
	PeripheralUnit();
	virtual ~PeripheralUnit();
  virtual void interruptHandler();
	bool    getInterruptFlag();
	void    clearInterruptFlag();
	static  void setIsrVectorTable(uint32_t *table);

protected:
	void init();
	void initIsr(IRQn_Type irq);

	bool isInitialized;
	bool interruptFlag;

private:
	static uint32_t *isrVectorTable;
};

#endif /* PERIPHERALUNIT_H_ */
