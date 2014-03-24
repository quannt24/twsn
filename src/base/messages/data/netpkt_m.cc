//
// Generated file, do not edit! Created by opp_msgc 4.3 from base/messages/data/netpkt.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "netpkt_m.h"

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




Register_Class(NetPkt);

NetPkt::NetPkt(const char *name, int kind) : cPacket(name,kind)
{
    this->pktSize_var = 16;
}

NetPkt::NetPkt(const NetPkt& other) : cPacket(other)
{
    copy(other);
}

NetPkt::~NetPkt()
{
}

NetPkt& NetPkt::operator=(const NetPkt& other)
{
    if (this==&other) return *this;
    cPacket::operator=(other);
    copy(other);
    return *this;
}

void NetPkt::copy(const NetPkt& other)
{
    this->srcAddr_var = other.srcAddr_var;
    this->desAddr_var = other.desAddr_var;
    this->pktSize_var = other.pktSize_var;
}

void NetPkt::parsimPack(cCommBuffer *b)
{
    cPacket::parsimPack(b);
    doPacking(b,this->srcAddr_var);
    doPacking(b,this->desAddr_var);
    doPacking(b,this->pktSize_var);
}

void NetPkt::parsimUnpack(cCommBuffer *b)
{
    cPacket::parsimUnpack(b);
    doUnpacking(b,this->srcAddr_var);
    doUnpacking(b,this->desAddr_var);
    doUnpacking(b,this->pktSize_var);
}

netaddr_t& NetPkt::getSrcAddr()
{
    return srcAddr_var;
}

void NetPkt::setSrcAddr(const netaddr_t& srcAddr)
{
    this->srcAddr_var = srcAddr;
}

netaddr_t& NetPkt::getDesAddr()
{
    return desAddr_var;
}

void NetPkt::setDesAddr(const netaddr_t& desAddr)
{
    this->desAddr_var = desAddr;
}

int NetPkt::getPktSize() const
{
    return pktSize_var;
}

void NetPkt::setPktSize(int pktSize)
{
    this->pktSize_var = pktSize;
}

class NetPktDescriptor : public cClassDescriptor
{
  public:
    NetPktDescriptor();
    virtual ~NetPktDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(NetPktDescriptor);

NetPktDescriptor::NetPktDescriptor() : cClassDescriptor("NetPkt", "cPacket")
{
}

NetPktDescriptor::~NetPktDescriptor()
{
}

bool NetPktDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<NetPkt *>(obj)!=NULL;
}

const char *NetPktDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int NetPktDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
}

unsigned int NetPktDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *NetPktDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "srcAddr",
        "desAddr",
        "pktSize",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int NetPktDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "srcAddr")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "desAddr")==0) return base+1;
    if (fieldName[0]=='p' && strcmp(fieldName, "pktSize")==0) return base+2;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *NetPktDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "netaddr_t",
        "netaddr_t",
        "int",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : NULL;
}

const char *NetPktDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int NetPktDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    NetPkt *pp = (NetPkt *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string NetPktDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    NetPkt *pp = (NetPkt *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getSrcAddr(); return out.str();}
        case 1: {std::stringstream out; out << pp->getDesAddr(); return out.str();}
        case 2: return long2string(pp->getPktSize());
        default: return "";
    }
}

bool NetPktDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    NetPkt *pp = (NetPkt *)object; (void)pp;
    switch (field) {
        case 2: pp->setPktSize(string2long(value)); return true;
        default: return false;
    }
}

const char *NetPktDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        "netaddr_t",
        "netaddr_t",
        NULL,
    };
    return (field>=0 && field<3) ? fieldStructNames[field] : NULL;
}

void *NetPktDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    NetPkt *pp = (NetPkt *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getSrcAddr()); break;
        case 1: return (void *)(&pp->getDesAddr()); break;
        default: return NULL;
    }
}


