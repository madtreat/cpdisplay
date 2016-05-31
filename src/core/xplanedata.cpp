/*
 * File:   xplanedata.h
 * Author: Madison Treat <madison.treat@tamu.edu>
 *
 * Created on June 18, 2015, 10:50 AM
 */

#include "xplanedata.h"
#include <QDebug>


QByteArray reverse(const QByteArray &ba) {
  QByteArray reverse;
  reverse.reserve(ba.size());
  for(int i=ba.size(); i>=0; --i) reverse.append(ba.at(i));
  return reverse;
}


XPOutputData::XPOutputData(const XPOutputData& other) {
  index = other.index;
  type = other.type;
  values = other.values;
}


/*
 * The data all comes in little endian format (smallest bytes first).
 * For the index, this does not matter, as nothing should be larger than 1 byte,
 * so the first byte should always be sufficient.
 * This does, however, matter for the actual data.
 */
void XPOutputData::parseRawData(QByteArray raw) {
  QByteArray indexRaw = raw.mid(0, 4);      // returns first 4 bytes
  QByteArray valuesRaw = raw.remove(0, 4);  // removes first 4 bytes

  // get the data index
  int in = (int) indexRaw.at(0);
  index = (XPDataIndex) in;

  // If ___, convert to int, else convert to double
  int NUM_VALUES = 8; // 8 values per data entry
  for (int i = 0; i < NUM_VALUES; i++) {
    QByteArray valueRaw = raw.mid(i*4, 4); // grab this set of 4 bytes
    QByteArray valueBytes = valueRaw;
    float* value = (float*) valueRaw.data();

    type = DOUBLE;

    values.append(*value);
  }
}

//QDebug& operator<<(QDebug& os, const XPData& data)
//void XPData::print() {
//  os << "XPData index:" << data.index;
//  for (int i = 0; i < data.values.size(); i++) {
//     os << "   value[" << i << "]:" << data.values.at(i);
//  }
//  return os;
//}

void XPOutputData::print() {
  qDebug() << "XPData index:" << (int) index << "of type" << (int) type;
  for (int i = 0; i < values.size(); i++) {
    qDebug() << "   value[" << i << "]:" << values.at(i);
  }
}