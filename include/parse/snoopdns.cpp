#include <SnoopDns>
#include <VDebugNew>

// ----------------------------------------------------------------------------
// SnoopDnsQuestion
// ----------------------------------------------------------------------------
QByteArray SnoopDnsQuestion::encode()
{
  QByteArray res;

  res = SnoopDns::encodeName(this->name);

  UINT16 _type = htons(this->type);
  res.append((const char*)&_type, sizeof(UINT16));

  UINT16 __class = htons(this->_class);
  res.append((const char*)&__class, sizeof(UINT16));

  return res;
}

bool SnoopDnsQuestion::decode(BYTE* udpData, int dataLen, int* offset)
{
  this->name = SnoopDns::decodeName(udpData, dataLen, offset);
  if (this->name == "") return false;

  if (*offset >= dataLen) return false;
  UINT16* _type = (UINT16*)(udpData + *offset);
  this->type = ntohs(*_type);
  *offset += sizeof(UINT16);

  if (*offset >= dataLen) return false;
  UINT16* __class = (UINT16*)(udpData + *offset);
  this->_class = ntohs(*__class);
  *offset += sizeof(UINT16);

  if (*offset >= dataLen) return false;
  return true;
}

// ----------------------------------------------------------------------------
// SnoopDnsQuestions
// ----------------------------------------------------------------------------
QByteArray SnoopDnsQuestions::encode()
{
  QByteArray res;
  foreach (SnoopDnsQuestion question, *this)
  {
    res += question.encode();
  }
  return res;
}

bool SnoopDnsQuestions::decode(BYTE* udpData, int dataLen, int count, int* offset)
{
  for (int i = 0; i < count; i++)
  {
    SnoopDnsQuestion question;
    if (!question.decode(udpData, dataLen, offset)) return false;
    push_back(question);
  }
  return true;
}

// ----------------------------------------------------------------------------
// SnoopDnsResourceRecord
// ----------------------------------------------------------------------------
QByteArray SnoopDnsResourceRecord::encode()
{
  QByteArray res;

  res = SnoopDns::encodeName(this->name);

  UINT16 _type = htons(this->type);
  res.append((const char*)&_type, sizeof(UINT16));

  UINT16 __class = htons(this->_class);
  res.append((const char*)&__class, sizeof(UINT16));

  UINT32 _ttl = htonl(this->ttl);
  res.append((const char*)&_ttl, sizeof(UINT32));

  UINT16 _dataLength = htons(this->dataLength);
  res.append((const char*)&_dataLength, sizeof(UINT16));

  res += data;

  return res;
}

bool SnoopDnsResourceRecord::decode(BYTE* udpData, int dataLen, int* offset)
{
  this->name = SnoopDns::decodeName(udpData, dataLen, offset);
  if (this->name == "") return false;

  if (*offset >= dataLen) return false;
  UINT16* _type = (UINT16*)(udpData + *offset);
  this->type = ntohs(*_type);
  *offset += sizeof(UINT16);

  if (*offset >= dataLen) return false;
  UINT16* __class = (UINT16*)(udpData + *offset);
  this->_class = ntohs(*__class);
  *offset += sizeof(UINT16);

  if (*offset >= dataLen) return false;
  UINT32* _ttl = (UINT32*)(udpData + *offset);
  this->ttl = ntohl(*_ttl);
  *offset += sizeof(UINT32);

  if (*offset >= dataLen) return false;
  UINT16* _dataLength = (UINT16*)(udpData + *offset);
  this->dataLength= ntohs(*_dataLength);
  *offset += sizeof(UINT32);

  if (*offset >= dataLen) return false;
  const char* data = (const char*)(udpData + *offset);
  this->data = QByteArray::fromRawData(data, dataLen - *offset);

  return true;
}

// ----------------------------------------------------------------------------
// SnoopDnsResourceRecords
// ----------------------------------------------------------------------------
QByteArray SnoopDnsResourceRecords::encode()
{
  QByteArray res;
  foreach (SnoopDnsResourceRecord record, *this)
  {
    res += record.encode();
  }
  return res;
}

bool SnoopDnsResourceRecords::decode(BYTE* udpData, int dataLen, int count, int* offset)
{
  for (int i = 0; i < count; i++)
  {
    SnoopDnsResourceRecord record;
    if (!record.decode(udpData, dataLen, offset)) return false;
    push_back(record);
  }
  return true;
}

// ----------------------------------------------------------------------------
// SnoopDns
// ----------------------------------------------------------------------------
QByteArray SnoopDns::encode()
{
  QByteArray res;

  UINT16 _id = htons(dnsHdr.id);
  res.append((const char*)&_id, sizeof(UINT16));

  UINT16 _flags = htons(dnsHdr.flags);
  res.append((const char*)&_flags, sizeof(UINT16));

  UINT16 _num_q = htons(dnsHdr.num_q);
  res.append((const char*)&_num_q, sizeof(UINT16));

  UINT16 _num_answ_rr = htons(dnsHdr.num_answ_rr);
  res.append((const char*)&_num_answ_rr, sizeof(UINT16));

  UINT16 _num_auth_rr = htons(dnsHdr.num_auth_rr);
  res.append((const char*)&_num_auth_rr, sizeof(UINT16));

  UINT16 _num_addi_rr = htons(dnsHdr.num_addi_rr);
  res.append((const char*)&_num_addi_rr, sizeof(UINT16));

  res += questions.encode();
  res += answers.encode();
  res += authorities.encode();
  res += additionals.encode();

  return res;
}

bool SnoopDns::decode(BYTE* udpData, int dataLen, int* offset)
{
  if (*offset >= dataLen) return false;
  UINT16* _id = (UINT16*)(udpData + *offset);
  this->dnsHdr.id = ntohs(*_id);
  *offset += sizeof(UINT16);

  if (*offset >= dataLen) return false;
  UINT16* _flags = (UINT16*)(udpData + *offset);
  this->dnsHdr.flags = ntohs(*_flags);
  *offset += sizeof(UINT16);

  if (*offset >= dataLen) return false;
  UINT16* _num_q = (UINT16*)(udpData + *offset);
  this->dnsHdr.num_q = ntohs(*_num_q);
  *offset += sizeof(UINT16);

  if (*offset >= dataLen) return false;
  UINT16* _num_answ_rr = (UINT16*)(udpData + *offset);
  this->dnsHdr.num_answ_rr = ntohs(*_num_answ_rr);
  *offset += sizeof(UINT16);

  if (*offset >= dataLen) return false;
  UINT16* _num_auth_rr = (UINT16*)(udpData + *offset);
  this->dnsHdr.num_auth_rr = ntohs(*_num_auth_rr);
  *offset += sizeof(UINT16);

  if (*offset >= dataLen) return false;
  UINT16* _num_addi_rr = (UINT16*)(udpData + *offset);
  this->dnsHdr.num_addi_rr = ntohs(*_num_addi_rr);
  *offset += sizeof(UINT16);

  if (questions.decode(udpData, dataLen, dnsHdr.num_q, offset)) return false;
  if (answers.decode(udpData, dataLen, dnsHdr.num_answ_rr, offset)) return false;
  if (authorities.decode(udpData, dataLen, dnsHdr.num_auth_rr, offset)) return false;
  if (additionals.decode(udpData, dataLen, dnsHdr.num_addi_rr, offset)) return false;

  return true;
}

QByteArray SnoopDns::encodeName(QString name)
{
  QStringList labels = name.split('.');
  int count = labels.count();
  if (count == 0)
  {
    LOG_WARN("label count is zero(%s)", qPrintable(name));
    return "";
  }

  QByteArray res;
  for (int i = 0; i < count - 2; i++)
  {
    QString label = labels.at(i);
    res += label;
    res += ".";
  }
  res += labels.at(count - 1);

  return res;
}

QString SnoopDns::decodeName(BYTE* udpData, int dataLen, int* offset)
{

}
