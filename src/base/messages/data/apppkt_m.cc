//
// Generated file, do not edit! Created by opp_msgc 4.3 from base/messages/data/apppkt.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "apppkt_m.h"

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
    cEnum *e = cEnum::find("RoutingType");
    if (!e) enums.getInstance()->add(e = new cEnum("RoutingType"));
    e->insert(RT_TO_AN, "RT_TO_AN");
    e->insert(RT_TO_BS, "RT_TO_BS");
    e->insert(RT_BROADCAST, "RT_BROADCAST");
);

Register_Class(AppPkt);

AppPkt::AppPkt(const char *name, int kind) : cPacket(name,kind)
{
    this->routingType_var = RT_TO_AN;
    this->pktSize_var = 1;
}

AppPkt::AppPkt(const AppPkt& other) : cPacket(other)
{
    copy(other);
}

AppPkt::~AppPkt()
{
}

AppPkt& AppPkt::operator=(const AppPkt& other)
{
    if (this==&other) return *this;
    cPacket::operator=(other);
    copy(other);
    return *this;
}

void AppPkt::copy(const AppPkt& other)
{
    this->srcNetAddr_var = other.srcNetAddr_var;
    this->desNetAddr_var = other.desNetAddr_var;
    this->routingType_var = other.routingType_var;
    this->pktSize_var = other.pktSize_var;
}

void AppPkt::parsimPack(cCommBuffer *b)
{
    cPacket::parsimPack(b);
    doPacking(b,this->srcNetAddr_var);
    doPacking(b,this->desNetAddr_var);
    doPacking(b,this->routingType_var);
    doPacking(b,this->pktSize_var);
}

void AppPkt::parsimUnpack(cCommBuffer *b)
{
    cPacket::parsimUnpack(b);
    doUnpacking(b,this->srcNetAddr_var);
    doUnpacking(b,this->desNetAddr_var);
    doUnpacking(b,this->routingType_var);
    doUnpacking(b,this->pktSize_var);
}

netaddr_t& AppPkt::getSrcNetAddr()
{
    return srcNetAddr_var;
}

void AppPkt::setSrcNetAddr(const netaddr_t& srcNetAddr)
{
    this->srcNetAddr_var = srcNetAddr;
}

netaddr_t& AppPkt::getDesNetAddr()
{
    return desNetAddr_var;
}

void AppPkt::setDesNetAddr(const netaddr_t& desNetAddr)
{
    this->desNetAddr_var = desNetAddr;
}

int AppPkt::getRoutingType() const
{
    return routingType_var;
}

void AppPkt::setRoutingType(int routingType)
{
    this->routingType_var = routingType;
}

int AppPkt::getPktSize() const
{
    return pktSize_var;
}

void AppPkt::setPktSize(int pktSize)
{
    this->pktSize_var = pktSize;
}

class AppPktDescriptor : public cClassDescriptor
{
  public:
    AppPktDescriptor();
    virtual ~AppPktDescriptor();

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

Register_ClassDescriptor(AppPktDescriptor);

AppPktDescriptor::AppPktDescriptor() : cClassDescriptor("AppPkt", "cPacket")
{
}

AppPktDescriptor::~AppPktDescriptor()
{
}

bool AppPktDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<AppPkt *>(obj)!=NULL;
}

const char *AppPktDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int AppPktDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount(object) : 4;
}

unsigned int AppPktDescriptor::getFieldTypeFlags(void *object, int field) const
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
        FD_ISEDITABLE,
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *AppPktDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "srcNetAddr",
        "desNetAddr",
        "routingType",
        "pktSize",
    };
    return (field>=0 && field<4) ? fieldNames[field] : NULL;
}

int AppPktDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "srcNetAddr")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "desNetAddr")==0) return base+1;
    if (fieldName[0]=='r' && strcmp(fieldName, "routingType")==0) return base+2;
    if (fieldName[0]=='p' && strcmp(fieldName, "pktSize")==0) return base+3;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *AppPktDescriptor::getFieldTypeString(void *object, int field) const
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
        "int",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : NULL;
}

const char *AppPktDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int AppPktDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    AppPkt *pp = (AppPkt *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string AppPktDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    AppPkt *pp = (AppPkt *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getSrcNetAddr(); return out.str();}
        case 1: {std::stringstream out; out << pp->getDesNetAddr(); return out.str();}
        case 2: return long2string(pp->getRoutingType());
        case 3: return long2string(pp->getPktSize());
        default: return "";
    }
}

bool AppPktDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    AppPkt *pp = (AppPkt *)object; (void)pp;
    switch (field) {
        case 2: pp->setRoutingType(string2long(value)); return true;
        case 3: pp->setPktSize(string2long(value)); return true;
        default: return false;
    }
}

const char *AppPktDescriptor::getFieldStructName(void *object, int field) const
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
        NULL,
    };
    return (field>=0 && field<4) ? fieldStructNames[field] : NULL;
}

void *AppPktDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    AppPkt *pp = (AppPkt *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getSrcNetAddr()); break;
        case 1: return (void *)(&pp->getDesNetAddr()); break;
        default: return NULL;
    }
}


