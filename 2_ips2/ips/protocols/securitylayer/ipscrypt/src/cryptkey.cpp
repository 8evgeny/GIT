/*
C++ class wrapper for store and modify cryptographic keys byte arrays.
Used as part of GIT-Comm IPS project.

        Copyright (c) 2020 Donskih Dmitriy

        Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"), to
deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished
to do so, subject to the following conditions:

        -The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

        THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

        Copyright (c) 2020 Донских Дмитрий

        Данная лицензия разрешает лицам, получившим копию данного программного
обеспечения и сопутствующей документации (в дальнейшем именуемыми «Программное
обеспечение»), безвозмездно использовать Программное обеспечение без
ограничений, включая неограниченное право на использование, копирование,
изменение, слияние, публикацию, распространение, сублицензирование и/или продажу
копий Программного обеспечения, а также лицам, которым предоставляется данное
Программное обеспечение, при соблюдении следующих условий:

        -Указанное выше уведомление об авторском праве и данные условия должны быть
включены во все копии или значимые части данного Программного обеспечения.

        ДАННОЕ ПРОГРАММНОЕ ОБЕСПЕЧЕНИЕ ПРЕДОСТАВЛЯЕТСЯ «КАК ЕСТЬ», БЕЗ КАКИХ-ЛИБО
ГАРАНТИЙ, ЯВНО ВЫРАЖЕННЫХ ИЛИ ПОДРАЗУМЕВАЕМЫХ, ВКЛЮЧАЯ ГАРАНТИИ ТОВАРНОЙ
ПРИГОДНОСТИ, СООТВЕТСТВИЯ ПО ЕГО КОНКРЕТНОМУ НАЗНАЧЕНИЮ И ОТСУТСТВИЯ НАРУШЕНИЙ,
НО НЕ ОГРАНИЧИВАЯСЬ ИМИ. НИ В КАКОМ СЛУЧАЕ АВТОРЫ ИЛИ ПРАВООБЛАДАТЕЛИ НЕ НЕСУТ
ОТВЕТСТВЕННОСТИ ПО КАКИМ-ЛИБО ИСКАМ, ЗА УЩЕРБ ИЛИ ПО ИНЫМ ТРЕБОВАНИЯМ, В ТОМ
ЧИСЛЕ, ПРИ ДЕЙСТВИИ КОНТРАКТА, ДЕЛИКТЕ ИЛИ ИНОЙ СИТУАЦИИ, ВОЗНИКШИМ ИЗ-ЗА
ИСПОЛЬЗОВАНИЯ ПРОГРАММНОГО ОБЕСПЕЧЕНИЯ ИЛИ ИНЫХ ДЕЙСТВИЙ С ПРОГРАММНЫМ
ОБЕСПЕЧЕНИЕМ.
*/

#include "cryptkey.h"
#include <cstring>

cryptkey::cryptkey() { memset(data, 0, KEY_SIZE); }

cryptkey::cryptkey(const uint8_t *key) { memcpy(data, key, KEY_SIZE); }

////////////////////////////////////////////////////////////////////////
// List of overloaded copy and block XOR operations                   //
////////////////////////////////////////////////////////////////////////

cryptkey &cryptkey::operator=(const cryptkey &rv) {
  memcpy(data, rv.data, KEY_SIZE);
  return *this;
}

cryptkey &cryptkey::operator=(const cryptkey *rv) {
  memcpy(data, rv->data, KEY_SIZE);
  return *this;
}

cryptkey &cryptkey::operator=(const uint8_t *rv) {
  memcpy(data, rv, KEY_SIZE);
  return *this;
}

cryptkey &cryptkey::operator^=(const cryptkey &rv) {
  for (int i = 0; i < KEY_SIZE; i++) {
    data[i] ^= rv.data[i];
  }
  return *this;
}

cryptkey &cryptkey::operator^=(const uint8_t *rv) {
  for (int i = 0; i < KEY_SIZE; i++) {
    data[i] ^= rv[i];
  }
  return *this;
}

cryptkey operator^(cryptkey lv, const uint8_t *rv) {
  lv ^= rv;
  return lv;
}

cryptkey operator^(cryptkey lv, const cryptkey &rv) {
  lv ^= rv;
  return lv;
}
