//
// Generated file, do not edit! Created by opp_msgc 4.4 from base/messages/mac802154pkt.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "mac802154pkt_m.h"

USING_NAMESPACE

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



namespace twsn {

EXECUTE_ON_STARTUP(
    cEnum *e = cEnum::find("twsn::Mac802154PktType");
    if (!e) enums.getInstance()->add(e = new cEnum("twsn::Mac802154PktType"));
    e->insert(MAC802154_BEACON, "MAC802154_BEACON");
    e->insert(MAC802154_DATA, "MAC802154_DATA");
    e->insert(MAC802154_ACK, "MAC802154_ACK");
    e->insert(MAC802154_CMD, "MAC802154_CMD");
    e->insert(MAC802154_PREAMBLE, "MAC802154_PREAMBLE");
);

Register_Class(Mac802154Pkt);

Mac802154Pkt::Mac802154Pkt(const char *name, int kind) : ::twsn::MacPkt(name,kind)
{
    this->setPktSize(21);

    this->pktType_var = MAC802154_DATA;
}

Mac802154Pkt::Mac802154Pkt(const Mac802154Pkt& other) : ::twsn::MacPkt(other)
{
    copy(other);
}

Mac802154Pkt::~Mac802154Pkt()
{
}

Mac802154Pkt& Mac802154Pkt::operator=(const Mac802154Pkt& other)
{
    if (this==&other) return *this;
    ::twsn::MacPkt::operator=(other);
    copy(other);
    return *this;
}

void Mac802154Pkt::copy(const Mac802154Pkt& other)
{
    this->pktType_var = other.pktType_var;
}

void Mac802154Pkt::parsimPack(cCommBuffer *b)
{
    ::twsn::MacPkt::parsimPack(b);
    doPacking(b,this->pktType_var);
}

void Mac802154Pkt::parsimUnpack(cCommBuffer *b)
{
    ::twsn::MacPkt::parsimUnpack(b);
    doUnpacking(b,this->pktType_var);
}

int Mac802154Pkt::getPktType() const
{
    return pktType_var;
}

void Mac802154Pkt::setPktType(int pktType)
{
    this->pktType_var = pktType;
}

class Mac802154PktDescriptor : public cClassDescriptor
{
  public:
    Mac802154PktDescriptor();
    virtual ~Mac802154PktDescriptor();

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

Register_ClassDescriptor(Mac802154PktDescriptor);

Mac802154PktDescriptor::Mac802154PktDescriptor() : cClassDescriptor("twsn::Mac802154Pkt", "twsn::MacPkt")
{
}

Mac802154PktDescriptor::~Mac802154PktDescriptor()
{
}

bool Mac802154PktDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<Mac802154Pkt *>(obj)!=NULL;
}

const char *Mac802154PktDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int Mac802154PktDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int Mac802154PktDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *Mac802154PktDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "pktType",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int Mac802154PktDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "pktType")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *Mac802154PktDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *Mac802154PktDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int Mac802154PktDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    Mac802154Pkt *pp = (Mac802154Pkt *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string Mac802154PktDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    Mac802154Pkt *pp = (Mac802154Pkt *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getPktType());
        default: return "";
    }
}

bool Mac802154PktDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    Mac802154Pkt *pp = (Mac802154Pkt *)object; (void)pp;
    switch (field) {
        case 0: pp->setPktType(string2long(value)); return true;
        default: return false;
    }
}

const char *Mac802154PktDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
    };
    return (field>=0 && field<1) ? fieldStructNames[field] : NULL;
}

void *Mac802154PktDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    Mac802154Pkt *pp = (Mac802154Pkt *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

}; // end namespace twsn

