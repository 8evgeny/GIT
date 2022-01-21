/*!
 \file packageClass.h

*/

#ifndef PACKAGECLASS_H
#define PACKAGECLASS_H

#include <cstdint>
/*!
  \brief The structure stores custom protocol fields

  \class Package packageClass.h "packageClass.h"
 */
using PackageTx = struct  {

    /*custom protocol fields*/
    uint8_t packetType,     /**< packet type */
            payloadData;
};

/*!
  \brief The structure stores custom protocol fields

  \class Package packageClass.h "packageClass.h"
 */
using PackageRx = struct  {

    /*custom protocol fields*/
    uint8_t packetType,     /**< packet type */
            sequence = 0; /**< packet number */
    uint16_t  payloadData[5];
};


/*!
 \brief The union stores PackageTx type for byte access to this type

 \class uPackageTx packageClass.h "packageClass.h"
*/
union uPackageTx {

    /*!
     \brief Base constructor. A empty constructor

     \fn uPackageTx
    */
    uPackageTx() {}
    PackageTx s;    /*!< The object of the PackageTx type */
    char b[2];      /*!< The array for the byte access to the [s] object */
};

/*!
 \brief The union stores PackageRx type for byte access to this type

 \class uPackageRx packageClass.h "packageClass.h"
*/
union uPackageRx {

    /*!
     \brief Base constructor. A empty constructor

     \fn uPackageRx
    */
    uPackageRx() {}
    PackageRx s; /*!< The object of the PackageTx type */
    char b[12]; /*!< The array for the byte access to the [s] object */
};

#endif // PACKAGECLASS_H
