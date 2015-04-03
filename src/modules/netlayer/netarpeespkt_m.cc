//
// Generated file, do not edit! Created by opp_msgc 4.4 from modules/netlayer/netarpeespkt.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "netarpeespkt_m.h"

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
    cEnum *e = cEnum::find("twsn::ArpeesPktType");
    if (!e) enums.getInstance()->add(e = new cEnum("twsn::ArpeesPktType"));
    e->insert(ARPEES_PAYLOAD_TO_AN, "ARPEES_PAYLOAD_TO_AN");
    e->insert(ARPEES_PAYLOAD_TO_BS, "ARPEES_PAYLOAD_TO_BS");
    e->insert(ARPEES_RELAY_REQ, "ARPEES_RELAY_REQ");
    e->insert(ARPEES_RELAY_INFO, "ARPEES_RELAY_INFO");
);

Register_Class(NetArpeesPkt);

NetArpeesPkt::NetArpeesPkt(const char *name, int kind) : ::twsn::NetPkt(name,kind)
{
    this->setPktSize(18);

    this->pktType_var = ARPEES_PAYLOAD_TO_AN;
}

NetArpeesPkt::NetArpeesPkt(const NetArpeesPkt& other) : ::twsn::NetPkt(other)
{
    copy(other);
}

NetArpeesPkt::~NetArpeesPkt()
{
}

NetArpeesPkt& NetArpeesPkt::operator=(const NetArpeesPkt& other)
{
    if (this==&other) return *this;
    ::twsn::NetPkt::operator=(other);
    copy(other);
    return *this;
}

void NetArpeesPkt::copy(const NetArpeesPkt& other)
{
    this->pktType_var = other.pktType_var;
}

void NetArpeesPkt::parsimPack(cCommBuffer *b)
{
    ::twsn::NetPkt::parsimPack(b);
    doPacking(b,this->pktType_var);
}

void NetArpeesPkt::parsimUnpack(cCommBuffer *b)
{
    ::twsn::NetPkt::parsimUnpack(b);
    doUnpacking(b,this->pktType_var);
}

int NetArpeesPkt::getPktType() const
{
    return pktType_var;
}

void NetArpeesPkt::setPktType(int pktType)
{
    this->pktType_var = pktType;
}

class NetArpeesPktDescriptor : public cClassDescriptor
{
  public:
    NetArpeesPktDescriptor();
    virtual ~NetArpeesPktDescriptor();

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

Register_ClassDescriptor(NetArpeesPktDescriptor);

NetArpeesPktDescriptor::NetArpeesPktDescriptor() : cClassDescriptor("twsn::NetArpeesPkt", "twsn::NetPkt")
{
}

NetArpeesPktDescriptor::~NetArpeesPktDescriptor()
{
}

bool NetArpeesPktDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<NetArpeesPkt *>(obj)!=NULL;
}

const char *NetArpeesPktDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int NetArpeesPktDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int NetArpeesPktDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *NetArpeesPktDescriptor::getFieldName(void *object, int field) const
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

int NetArpeesPktDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "pktType")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *NetArpeesPktDescriptor::getFieldTypeString(void *object, int field) const
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

const char *NetArpeesPktDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int NetArpeesPktDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    NetArpeesPkt *pp = (NetArpeesPkt *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string NetArpeesPktDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    NetArpeesPkt *pp = (NetArpeesPkt *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getPktType());
        default: return "";
    }
}

bool NetArpeesPktDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    NetArpeesPkt *pp = (NetArpeesPkt *)object; (void)pp;
    switch (field) {
        case 0: pp->setPktType(string2long(value)); return true;
        default: return false;
    }
}

const char *NetArpeesPktDescriptor::getFieldStructName(void *object, int field) const
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

void *NetArpeesPktDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    NetArpeesPkt *pp = (NetArpeesPkt *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(NetArpeesRelayInfoPkt);

NetArpeesRelayInfoPkt::NetArpeesRelayInfoPkt(const char *name, int kind) : ::twsn::NetArpeesPkt(name,kind)
{
    this->setPktSize(34);
    this->setPktType(ARPEES_RELAY_INFO);

    this->bsFlag_var = false;
    this->energy_var = 0;
    this->posX_var = 0;
    this->posY_var = 0;
    this->dBs_var = 0;
}

NetArpeesRelayInfoPkt::NetArpeesRelayInfoPkt(const NetArpeesRelayInfoPkt& other) : ::twsn::NetArpeesPkt(other)
{
    copy(other);
}

NetArpeesRelayInfoPkt::~NetArpeesRelayInfoPkt()
{
}

NetArpeesRelayInfoPkt& NetArpeesRelayInfoPkt::operator=(const NetArpeesRelayInfoPkt& other)
{
    if (this==&other) return *this;
    ::twsn::NetArpeesPkt::operator=(other);
    copy(other);
    return *this;
}

void NetArpeesRelayInfoPkt::copy(const NetArpeesRelayInfoPkt& other)
{
    this->bsFlag_var = other.bsFlag_var;
    this->energy_var = other.energy_var;
    this->posX_var = other.posX_var;
    this->posY_var = other.posY_var;
    this->dBs_var = other.dBs_var;
}

void NetArpeesRelayInfoPkt::parsimPack(cCommBuffer *b)
{
    ::twsn::NetArpeesPkt::parsimPack(b);
    doPacking(b,this->bsFlag_var);
    doPacking(b,this->energy_var);
    doPacking(b,this->posX_var);
    doPacking(b,this->posY_var);
    doPacking(b,this->dBs_var);
}

void NetArpeesRelayInfoPkt::parsimUnpack(cCommBuffer *b)
{
    ::twsn::NetArpeesPkt::parsimUnpack(b);
    doUnpacking(b,this->bsFlag_var);
    doUnpacking(b,this->energy_var);
    doUnpacking(b,this->posX_var);
    doUnpacking(b,this->posY_var);
    doUnpacking(b,this->dBs_var);
}

bool NetArpeesRelayInfoPkt::getBsFlag() const
{
    return bsFlag_var;
}

void NetArpeesRelayInfoPkt::setBsFlag(bool bsFlag)
{
    this->bsFlag_var = bsFlag;
}

double NetArpeesRelayInfoPkt::getEnergy() const
{
    return energy_var;
}

void NetArpeesRelayInfoPkt::setEnergy(double energy)
{
    this->energy_var = energy;
}

double NetArpeesRelayInfoPkt::getPosX() const
{
    return posX_var;
}

void NetArpeesRelayInfoPkt::setPosX(double posX)
{
    this->posX_var = posX;
}

double NetArpeesRelayInfoPkt::getPosY() const
{
    return posY_var;
}

void NetArpeesRelayInfoPkt::setPosY(double posY)
{
    this->posY_var = posY;
}

double NetArpeesRelayInfoPkt::getDBs() const
{
    return dBs_var;
}

void NetArpeesRelayInfoPkt::setDBs(double dBs)
{
    this->dBs_var = dBs;
}

class NetArpeesRelayInfoPktDescriptor : public cClassDescriptor
{
  public:
    NetArpeesRelayInfoPktDescriptor();
    virtual ~NetArpeesRelayInfoPktDescriptor();

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

Register_ClassDescriptor(NetArpeesRelayInfoPktDescriptor);

NetArpeesRelayInfoPktDescriptor::NetArpeesRelayInfoPktDescriptor() : cClassDescriptor("twsn::NetArpeesRelayInfoPkt", "twsn::NetArpeesPkt")
{
}

NetArpeesRelayInfoPktDescriptor::~NetArpeesRelayInfoPktDescriptor()
{
}

bool NetArpeesRelayInfoPktDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<NetArpeesRelayInfoPkt *>(obj)!=NULL;
}

const char *NetArpeesRelayInfoPktDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int NetArpeesRelayInfoPktDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 5+basedesc->getFieldCount(object) : 5;
}

unsigned int NetArpeesRelayInfoPktDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<5) ? fieldTypeFlags[field] : 0;
}

const char *NetArpeesRelayInfoPktDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "bsFlag",
        "energy",
        "posX",
        "posY",
        "dBs",
    };
    return (field>=0 && field<5) ? fieldNames[field] : NULL;
}

int NetArpeesRelayInfoPktDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='b' && strcmp(fieldName, "bsFlag")==0) return base+0;
    if (fieldName[0]=='e' && strcmp(fieldName, "energy")==0) return base+1;
    if (fieldName[0]=='p' && strcmp(fieldName, "posX")==0) return base+2;
    if (fieldName[0]=='p' && strcmp(fieldName, "posY")==0) return base+3;
    if (fieldName[0]=='d' && strcmp(fieldName, "dBs")==0) return base+4;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *NetArpeesRelayInfoPktDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "bool",
        "double",
        "double",
        "double",
        "double",
    };
    return (field>=0 && field<5) ? fieldTypeStrings[field] : NULL;
}

const char *NetArpeesRelayInfoPktDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int NetArpeesRelayInfoPktDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    NetArpeesRelayInfoPkt *pp = (NetArpeesRelayInfoPkt *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string NetArpeesRelayInfoPktDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    NetArpeesRelayInfoPkt *pp = (NetArpeesRelayInfoPkt *)object; (void)pp;
    switch (field) {
        case 0: return bool2string(pp->getBsFlag());
        case 1: return double2string(pp->getEnergy());
        case 2: return double2string(pp->getPosX());
        case 3: return double2string(pp->getPosY());
        case 4: return double2string(pp->getDBs());
        default: return "";
    }
}

bool NetArpeesRelayInfoPktDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    NetArpeesRelayInfoPkt *pp = (NetArpeesRelayInfoPkt *)object; (void)pp;
    switch (field) {
        case 0: pp->setBsFlag(string2bool(value)); return true;
        case 1: pp->setEnergy(string2double(value)); return true;
        case 2: pp->setPosX(string2double(value)); return true;
        case 3: pp->setPosY(string2double(value)); return true;
        case 4: pp->setDBs(string2double(value)); return true;
        default: return false;
    }
}

const char *NetArpeesRelayInfoPktDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    };
    return (field>=0 && field<5) ? fieldStructNames[field] : NULL;
}

void *NetArpeesRelayInfoPktDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    NetArpeesRelayInfoPkt *pp = (NetArpeesRelayInfoPkt *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(NetArpeesResRelayInfoTimer);

NetArpeesResRelayInfoTimer::NetArpeesResRelayInfoTimer(const char *name, int kind) : ::cMessage(name,kind)
{
}

NetArpeesResRelayInfoTimer::NetArpeesResRelayInfoTimer(const NetArpeesResRelayInfoTimer& other) : ::cMessage(other)
{
    copy(other);
}

NetArpeesResRelayInfoTimer::~NetArpeesResRelayInfoTimer()
{
}

NetArpeesResRelayInfoTimer& NetArpeesResRelayInfoTimer::operator=(const NetArpeesResRelayInfoTimer& other)
{
    if (this==&other) return *this;
    ::cMessage::operator=(other);
    copy(other);
    return *this;
}

void NetArpeesResRelayInfoTimer::copy(const NetArpeesResRelayInfoTimer& other)
{
    this->reqAddr_var = other.reqAddr_var;
}

void NetArpeesResRelayInfoTimer::parsimPack(cCommBuffer *b)
{
    ::cMessage::parsimPack(b);
    doPacking(b,this->reqAddr_var);
}

void NetArpeesResRelayInfoTimer::parsimUnpack(cCommBuffer *b)
{
    ::cMessage::parsimUnpack(b);
    doUnpacking(b,this->reqAddr_var);
}

twsn::netaddr_t& NetArpeesResRelayInfoTimer::getReqAddr()
{
    return reqAddr_var;
}

void NetArpeesResRelayInfoTimer::setReqAddr(const twsn::netaddr_t& reqAddr)
{
    this->reqAddr_var = reqAddr;
}

class NetArpeesResRelayInfoTimerDescriptor : public cClassDescriptor
{
  public:
    NetArpeesResRelayInfoTimerDescriptor();
    virtual ~NetArpeesResRelayInfoTimerDescriptor();

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

Register_ClassDescriptor(NetArpeesResRelayInfoTimerDescriptor);

NetArpeesResRelayInfoTimerDescriptor::NetArpeesResRelayInfoTimerDescriptor() : cClassDescriptor("twsn::NetArpeesResRelayInfoTimer", "cMessage")
{
}

NetArpeesResRelayInfoTimerDescriptor::~NetArpeesResRelayInfoTimerDescriptor()
{
}

bool NetArpeesResRelayInfoTimerDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<NetArpeesResRelayInfoTimer *>(obj)!=NULL;
}

const char *NetArpeesResRelayInfoTimerDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int NetArpeesResRelayInfoTimerDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int NetArpeesResRelayInfoTimerDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *NetArpeesResRelayInfoTimerDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "reqAddr",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int NetArpeesResRelayInfoTimerDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='r' && strcmp(fieldName, "reqAddr")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *NetArpeesResRelayInfoTimerDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "netaddr_t",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *NetArpeesResRelayInfoTimerDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int NetArpeesResRelayInfoTimerDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    NetArpeesResRelayInfoTimer *pp = (NetArpeesResRelayInfoTimer *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string NetArpeesResRelayInfoTimerDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    NetArpeesResRelayInfoTimer *pp = (NetArpeesResRelayInfoTimer *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getReqAddr(); return out.str();}
        default: return "";
    }
}

bool NetArpeesResRelayInfoTimerDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    NetArpeesResRelayInfoTimer *pp = (NetArpeesResRelayInfoTimer *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *NetArpeesResRelayInfoTimerDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        "twsn::netaddr_t",
    };
    return (field>=0 && field<1) ? fieldStructNames[field] : NULL;
}

void *NetArpeesResRelayInfoTimerDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    NetArpeesResRelayInfoTimer *pp = (NetArpeesResRelayInfoTimer *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getReqAddr()); break;
        default: return NULL;
    }
}

}; // end namespace twsn

