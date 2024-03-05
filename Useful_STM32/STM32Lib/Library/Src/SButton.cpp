#include "SButton.h"
#include "STM32.h"

// ==================== CONSTRUCTOR ====================
SButton::SButton(GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN, bool type, bool dir)
{
  _GPIOx_IN = GPIOx;
  _GPIO_Pin_IN = GPIO_PIN;
  flags.noPin = false;

  if(type)
    _SetPin(_GPIOx_IN, _GPIO_Pin_IN, Input, Pull_Down);
  else
    _SetPin(_GPIOx_IN, _GPIO_Pin_IN, Input, Pull_Up);

  flags.type = type;
  flags.mode = false;
  flags.tickMode = true; //false;
  flags.inv_state = dir;
}

// ==================== SET ====================
void SButton::setDebounce(uint16_t debounce) { _debounce = debounce; }
void SButton::setTimeout(uint16_t new_timeout) { _timeout = new_timeout; }
void SButton::setClickTimeout(uint16_t new_timeout) { _click_timeout = new_timeout; }
void SButton::setStepTimeout(uint16_t step_timeout) { _step_timeout = step_timeout; }
void SButton::setDirection(bool dir) { flags.inv_state = dir; }
void SButton::setTickMode(bool tickMode) { flags.tickMode = tickMode; }

// ==================== IS ====================
bool SButton::isPress()
{
  if(flags.tickMode) SButton::tick();
  if(flags.isPress_f)
  {
    flags.isPress_f = false;
    return true;
  }
  else return false;
}

bool SButton::isRelease()
{
  if(flags.tickMode) SButton::tick();
  if(flags.isRelease_f)
  {
    flags.isRelease_f = false;
    return true;
  }
  else return false;
}

bool SButton::isClick()
{
  if(flags.tickMode) SButton::tick();
  if(flags.isOne_f)
  {
    flags.isOne_f = false;
    return true;
  }
  else return false;
}

bool SButton::isHolded()
{
  if(flags.tickMode) SButton::tick();
  if(flags.isHolded_f)
  {
    flags.isHolded_f = false;
    return true;
  }
  else return false;
}

bool SButton::isHold()
{
  if(flags.tickMode) SButton::tick();
  if(flags.step_flag) return true;
  else return false;
}

bool SButton::state()
{
  if(flags.tickMode) SButton::tick();
  return btn_state;
}

bool SButton::isSingle()
{
  if(flags.tickMode) SButton::tick();
  if(flags.counter_flag && last_counter == 1)
  {
//    last_counter = 0;
//    flags.counter_flag = false;
   flags.counter_reset = true; 
    return true;
  }
  else return false;
}

bool SButton::isDouble()
{
  if(flags.tickMode) SButton::tick();
  if(flags.counter_flag && last_counter == 2)
  {
//    flags.counter_flag = false;
//    last_counter = 0;
    flags.counter_reset = true;
    return true;
  }
  else return false;
}

bool SButton::isTriple()
{
  if(flags.tickMode) SButton::tick();
  if(flags.counter_flag && last_counter == 3)
  {
 //   flags.counter_flag = false;
 //   last_counter = 0;
    flags.counter_reset = true;
    return true;
  }
  else return false;
}

bool SButton::hasClicks()
{
  if(flags.tickMode) SButton::tick();
  if(flags.counter_flag)
  {
 //   flags.counter_flag = false;
    flags.counter_reset = true;
    return true;
  }
  else return false;
}

uint8_t SButton::getClicks()
{
  //uint8_t thisCount = last_counter;
  //last_counter = 0;
  flags.counter_reset = true;
  return last_counter;
}

uint8_t SButton::getHoldClicks()
{
  if(flags.tickMode) SButton::tick();
  return last_hold_counter;              //flags.hold_flag ? last_hold_counter : 0;
}

bool SButton::isStep(uint8_t clicks)
{
  if(flags.tickMode) SButton::tick();
  if(btn_counter == clicks && flags.step_flag && (millis() - btn_timer >= _step_timeout))
  {
    btn_timer = millis();
    return true;
  }
  else return false;
}

void SButton::resetStates()
{
  flags.isPress_f = false;
  flags.isRelease_f = false;
  flags.isOne_f = false;
  flags.isHolded_f = false;
  flags.step_flag = false;
  flags.counter_flag = false;
  last_hold_counter = 0;
  last_counter = 0;
}

// ==================== TICK ====================
void SButton::tick(bool state)
{
  flags.mode = true;
  btn_state = state ^ flags.inv_state;
  SButton::tick();
  flags.mode = false;
}

void SButton::tick()
{
  // читаем пин
  if(!flags.mode && !flags.noPin)
    btn_state = !_DigitalReadBit(_GPIOx_IN, _GPIO_Pin_IN) ^ (flags.inv_state ^ flags.type);

  uint32_t thisMls = millis();

  // нажатие
  if(btn_state && !btn_flag)
  {
    if(!flags.btn_deb)
    {
      flags.btn_deb = true;
      btn_timer = thisMls;
    }
    else
    {
      if(thisMls - btn_timer >= _debounce)
      {
        btn_flag = true;
        flags.isPress_f = true;
        flags.oneClick_f = true;
      }
    }
  }
  else
  {
    flags.btn_deb = false;
  }

  // отпускание
  if(!btn_state && btn_flag)
  {
    btn_flag = false;
    if(!flags.hold_flag) btn_counter++;
    flags.hold_flag = false;
    flags.isRelease_f = true;
    btn_timer = thisMls;
    if(flags.step_flag)
    {
      last_counter = 0;
      btn_counter = 0;
      flags.step_flag = false;
    }
    if(flags.oneClick_f)
    {
      flags.oneClick_f = false;
      flags.isOne_f = true;
    }
  }

  // кнопка удерживается
  if(btn_flag && btn_state && (thisMls - btn_timer >= _timeout) && !flags.hold_flag)
  {
    flags.hold_flag = true;
    last_hold_counter = btn_counter;
    //btn_counter = 0;
    //last_counter = 0;
    flags.isHolded_f = true;
    flags.step_flag = true;
    flags.oneClick_f = false;
    btn_timer = thisMls;
  }

  // обработка накликивания
  if((thisMls - btn_timer >= _click_timeout) && (btn_counter != 0) && !btn_state)
  {
    last_counter = btn_counter;
    btn_counter = 0;
    flags.counter_flag = true;
  }
    
    // сброс накликивания						//Добавлено
    if (flags.counter_reset) {
        last_counter = 0;
        flags.counter_flag = false;
        flags.counter_reset = false;
    }
}
