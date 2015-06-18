/* 
 * File:   xplanedata.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 18, 2015, 10:50 AM
 */

#include "xplanedata.h"


void XPData::parseRawData(QByteArray raw)
{
      QByteArray indexRaw = raw.mid(0, 4);        // returns first 4 bytes
      QByteArray valueRaw = raw.remove(0, 4);  // removes first 4 bytes
      
      // get the data index
      int i = (int) indexRaw.at(0);
      index = (XPDataIndex) i;
      
      // If ___, convert to int, else convert to double
}