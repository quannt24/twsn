//
// Generated file, do not edit! Created by opp_msgc 4.3 from base/messages/data/macpkt.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "macpkt_m.h"

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




EXECUTE_ON_STARTUP(
    cEnum *e = cEnum::find("MacPktType");
    if (!e) enums.getInstance()->add(e = new cEnum("MacPktType"));
    e->insert(MAC_PKT_PAYLOAD, "MAC_PKT_PAYLOAD");
    e->insert(MAC_PKT_CTL, "MAC_PKT_CTL");
    e->insert(MAC_PKT_PREAMBLE, "MAC_PKT_PREAMBLE");
    e->insert(MAC_PKT_ACK, "MAC_PKT_ACK");
);

Register_Class(MacPkt);

MacPkt::MacPkt(const char *name, int kind) : cPacket(name,kind)
{
    this->pktType_var = MAC_PKT_PAYLOAD;
    this->pktSize_var = 16;
}

MacPkt::MacPkt(const MacPkt& other) : cPacket(other)
{
    copy(other);
}

MacPkt::~MacPkt()
{
}

MacPkt& MacPkt::operator=(const MacPkt& other)
{
    if (this==&other) return *this;
    cPacket::operator=(other);
    copy(other);
    return *this;
}

void MacPkt::copy(const MacPkt& other)
{
    this->pktType_var = other.pktType_var;
    this->srcAddr_var = other.srcAddr_var;
    this->desAddr_var = other.desAddr_var;
    this->pktSize_var = other.pktSize_var;
}

void MacPkt::parsimPack(cCommBuffer *b)
{
    cPacket::parsimPack(b);
    doPacking(b,this->pktType_var);
    doPacking(b,this->srcAddr_var);
    doPacking(b,this->desAddr_var);
    doPacking(b,this->pktSize_var);
}

void MacPkt::parsimUnpack(cCommBuffer *b)
{
    cPacket::parsimUnpack(b);
    doUnpacking(b,this->pktType_var);
    doUnpacking(b,this->srcAddr_var);
    doUnpacking(b,this->desAddr_var);
    doUnpacking(b,this->pktSize_var);
}

int MacPkt::getPktType() const
{
    return pktType_var;
}

void MacPkt::setPktType(int pktType)
{
    this->pktType_var = pktType;
}

macaddr_t& MacPkt::getSrcAddr()
{
    return srcAddr_var;
}

void MacPkt::setSrcAddr(const macaddr_t& srcAddr)
{
    this->srcAddr_var = srcAddr;
}

macaddr_t& MacPkt::getDesAddr()
{
    return desAddr_var;
}

void MacPkt::setDesAddr(const macaddr_t& desAddr)
{
    this->desAddr_var = desAddr;
}

int MacPkt::getPktSize() const
{
    return pktSize_var;
}

void MacPkt::setPktSize(int pktSize)
{
    this->pktSize_var = pktSize;
}

class MacPktDescriptor : public cClassDescriptor
{
  public:
    MacPktDescriptor();
    virtual ~MacPktDescriptor();

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

Register_ClassDescriptor(MacPktDescriptor);

MacPktDescriptor::MacPktDescriptor() : cClassDescriptor("MacPkt", "cPacket")
{
}

MacPktDescriptor::~MacPktDescriptor()
{
}

bool MacPktDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<MacPkt *>(obj)!=NULL;
}

const char *MacPktDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int MacPktDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount(object) : 4;
}

unsigned int MacPktDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *MacPktDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "pktType",
        "srcAddr",
        "desAddr",
        "pktSize",
    };
    return (field>=0 && field<4) ? fieldNames[field] : NULL;
}

int MacPktDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "pktType")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "srcAddr")==0) return base+1;
    if (fieldName[0]=='d' && strcmp(fieldName, "desAddr")==0) return base+2;
    if (fieldName[0]=='p' && strcmp(fieldName, "pktSize")==0) return base+3;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *MacPktDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "macaddr_t",
        "macaddr_t",
        "int",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : NULL;
}

const char *MacPktDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int MacPktDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    MacPkt *pp = (MacPkt *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string MacPktDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    MacPkt *pp = (MacPkt *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getPktType());
        case 1: {std::stringstream out; out << pp->getSrcAddr(); return out.str();}
        case 2: {std::stringstream out; out << pp->getDesAddr(); return out.str();}
        case 3: return long2string(pp->getPktSize());
        default: return "";
    }
}

bool MacPktDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    MacPkt *pp = (MacPkt *)object; (void)pp;
    switch (field) {
        case 0: pp->setPktType(string2long(value)); return true;
        case 3: pp->setPktSize(string2long(value)); return true;
        default: return false;
    }
}

const char *MacPktDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        "macaddr_t",
        "macaddr_t",
        NULL,
    };
    return (field>=0 && field<4) ? fieldStructNames[field] : NULL;
}

void *MacPktDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    MacPkt *pp = (MacPkt *)object; (void)pp;
    switch (field) {
        case 1: return (void *)(&pp->getSrcAddr()); break;
        case 2: return (void *)(&pp->getDesAddr()); break;
        default: return NULL;
    }
}


