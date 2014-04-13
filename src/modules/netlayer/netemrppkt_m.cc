//
// Generated file, do not edit! Created by opp_msgc 4.4 from modules/netlayer/netemrppkt.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "netemrppkt_m.h"

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
    cEnum *e = cEnum::find("twsn::EmrpPktType");
    if (!e) enums.getInstance()->add(e = new cEnum("twsn::EmrpPktType"));
    e->insert(EMRP_PAYLOAD_TO_AN, "EMRP_PAYLOAD_TO_AN");
    e->insert(EMRP_PAYLOAD_TO_BS, "EMRP_PAYLOAD_TO_BS");
    e->insert(EMRP_RELAY_REQ, "EMRP_RELAY_REQ");
    e->insert(EMRP_RELAY_INFO, "EMRP_RELAY_INFO");
    e->insert(EMRP_ENERGY_INFO, "EMRP_ENERGY_INFO");
);

Register_Class(NetEmrpPkt);

NetEmrpPkt::NetEmrpPkt(const char *name, int kind) : ::twsn::NetPkt(name,kind)
{
    this->setPktSize(19);

    this->pktType_var = EMRP_PAYLOAD_TO_AN;
}

NetEmrpPkt::NetEmrpPkt(const NetEmrpPkt& other) : ::twsn::NetPkt(other)
{
    copy(other);
}

NetEmrpPkt::~NetEmrpPkt()
{
}

NetEmrpPkt& NetEmrpPkt::operator=(const NetEmrpPkt& other)
{
    if (this==&other) return *this;
    ::twsn::NetPkt::operator=(other);
    copy(other);
    return *this;
}

void NetEmrpPkt::copy(const NetEmrpPkt& other)
{
    this->pktType_var = other.pktType_var;
}

void NetEmrpPkt::parsimPack(cCommBuffer *b)
{
    ::twsn::NetPkt::parsimPack(b);
    doPacking(b,this->pktType_var);
}

void NetEmrpPkt::parsimUnpack(cCommBuffer *b)
{
    ::twsn::NetPkt::parsimUnpack(b);
    doUnpacking(b,this->pktType_var);
}

int NetEmrpPkt::getPktType() const
{
    return pktType_var;
}

void NetEmrpPkt::setPktType(int pktType)
{
    this->pktType_var = pktType;
}

class NetEmrpPktDescriptor : public cClassDescriptor
{
  public:
    NetEmrpPktDescriptor();
    virtual ~NetEmrpPktDescriptor();

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

Register_ClassDescriptor(NetEmrpPktDescriptor);

NetEmrpPktDescriptor::NetEmrpPktDescriptor() : cClassDescriptor("twsn::NetEmrpPkt", "twsn::NetPkt")
{
}

NetEmrpPktDescriptor::~NetEmrpPktDescriptor()
{
}

bool NetEmrpPktDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<NetEmrpPkt *>(obj)!=NULL;
}

const char *NetEmrpPktDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int NetEmrpPktDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int NetEmrpPktDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *NetEmrpPktDescriptor::getFieldName(void *object, int field) const
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

int NetEmrpPktDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "pktType")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *NetEmrpPktDescriptor::getFieldTypeString(void *object, int field) const
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

const char *NetEmrpPktDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int NetEmrpPktDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    NetEmrpPkt *pp = (NetEmrpPkt *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string NetEmrpPktDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    NetEmrpPkt *pp = (NetEmrpPkt *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getPktType());
        default: return "";
    }
}

bool NetEmrpPktDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    NetEmrpPkt *pp = (NetEmrpPkt *)object; (void)pp;
    switch (field) {
        case 0: pp->setPktType(string2long(value)); return true;
        default: return false;
    }
}

const char *NetEmrpPktDescriptor::getFieldStructName(void *object, int field) const
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

void *NetEmrpPktDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    NetEmrpPkt *pp = (NetEmrpPkt *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(NetEmrpRelayInfoPkt);

NetEmrpRelayInfoPkt::NetEmrpRelayInfoPkt(const char *name, int kind) : ::twsn::NetEmrpPkt(name,kind)
{
    this->setPktSize(35);
    this->setPktType(EMRP_RELAY_INFO);

    this->bsFlag_var = false;
    this->energy_var = 0;
    this->posX_var = 0;
    this->posY_var = 0;
    this->dBs_var = 0;
}

NetEmrpRelayInfoPkt::NetEmrpRelayInfoPkt(const NetEmrpRelayInfoPkt& other) : ::twsn::NetEmrpPkt(other)
{
    copy(other);
}

NetEmrpRelayInfoPkt::~NetEmrpRelayInfoPkt()
{
}

NetEmrpRelayInfoPkt& NetEmrpRelayInfoPkt::operator=(const NetEmrpRelayInfoPkt& other)
{
    if (this==&other) return *this;
    ::twsn::NetEmrpPkt::operator=(other);
    copy(other);
    return *this;
}

void NetEmrpRelayInfoPkt::copy(const NetEmrpRelayInfoPkt& other)
{
    this->bsFlag_var = other.bsFlag_var;
    this->energy_var = other.energy_var;
    this->posX_var = other.posX_var;
    this->posY_var = other.posY_var;
    this->dBs_var = other.dBs_var;
}

void NetEmrpRelayInfoPkt::parsimPack(cCommBuffer *b)
{
    ::twsn::NetEmrpPkt::parsimPack(b);
    doPacking(b,this->bsFlag_var);
    doPacking(b,this->energy_var);
    doPacking(b,this->posX_var);
    doPacking(b,this->posY_var);
    doPacking(b,this->dBs_var);
}

void NetEmrpRelayInfoPkt::parsimUnpack(cCommBuffer *b)
{
    ::twsn::NetEmrpPkt::parsimUnpack(b);
    doUnpacking(b,this->bsFlag_var);
    doUnpacking(b,this->energy_var);
    doUnpacking(b,this->posX_var);
    doUnpacking(b,this->posY_var);
    doUnpacking(b,this->dBs_var);
}

bool NetEmrpRelayInfoPkt::getBsFlag() const
{
    return bsFlag_var;
}

void NetEmrpRelayInfoPkt::setBsFlag(bool bsFlag)
{
    this->bsFlag_var = bsFlag;
}

double NetEmrpRelayInfoPkt::getEnergy() const
{
    return energy_var;
}

void NetEmrpRelayInfoPkt::setEnergy(double energy)
{
    this->energy_var = energy;
}

double NetEmrpRelayInfoPkt::getPosX() const
{
    return posX_var;
}

void NetEmrpRelayInfoPkt::setPosX(double posX)
{
    this->posX_var = posX;
}

double NetEmrpRelayInfoPkt::getPosY() const
{
    return posY_var;
}

void NetEmrpRelayInfoPkt::setPosY(double posY)
{
    this->posY_var = posY;
}

double NetEmrpRelayInfoPkt::getDBs() const
{
    return dBs_var;
}

void NetEmrpRelayInfoPkt::setDBs(double dBs)
{
    this->dBs_var = dBs;
}

class NetEmrpRelayInfoPktDescriptor : public cClassDescriptor
{
  public:
    NetEmrpRelayInfoPktDescriptor();
    virtual ~NetEmrpRelayInfoPktDescriptor();

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

Register_ClassDescriptor(NetEmrpRelayInfoPktDescriptor);

NetEmrpRelayInfoPktDescriptor::NetEmrpRelayInfoPktDescriptor() : cClassDescriptor("twsn::NetEmrpRelayInfoPkt", "twsn::NetEmrpPkt")
{
}

NetEmrpRelayInfoPktDescriptor::~NetEmrpRelayInfoPktDescriptor()
{
}

bool NetEmrpRelayInfoPktDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<NetEmrpRelayInfoPkt *>(obj)!=NULL;
}

const char *NetEmrpRelayInfoPktDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int NetEmrpRelayInfoPktDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 5+basedesc->getFieldCount(object) : 5;
}

unsigned int NetEmrpRelayInfoPktDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *NetEmrpRelayInfoPktDescriptor::getFieldName(void *object, int field) const
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

int NetEmrpRelayInfoPktDescriptor::findField(void *object, const char *fieldName) const
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

const char *NetEmrpRelayInfoPktDescriptor::getFieldTypeString(void *object, int field) const
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

const char *NetEmrpRelayInfoPktDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int NetEmrpRelayInfoPktDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    NetEmrpRelayInfoPkt *pp = (NetEmrpRelayInfoPkt *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string NetEmrpRelayInfoPktDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    NetEmrpRelayInfoPkt *pp = (NetEmrpRelayInfoPkt *)object; (void)pp;
    switch (field) {
        case 0: return bool2string(pp->getBsFlag());
        case 1: return double2string(pp->getEnergy());
        case 2: return double2string(pp->getPosX());
        case 3: return double2string(pp->getPosY());
        case 4: return double2string(pp->getDBs());
        default: return "";
    }
}

bool NetEmrpRelayInfoPktDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    NetEmrpRelayInfoPkt *pp = (NetEmrpRelayInfoPkt *)object; (void)pp;
    switch (field) {
        case 0: pp->setBsFlag(string2bool(value)); return true;
        case 1: pp->setEnergy(string2double(value)); return true;
        case 2: pp->setPosX(string2double(value)); return true;
        case 3: pp->setPosY(string2double(value)); return true;
        case 4: pp->setDBs(string2double(value)); return true;
        default: return false;
    }
}

const char *NetEmrpRelayInfoPktDescriptor::getFieldStructName(void *object, int field) const
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

void *NetEmrpRelayInfoPktDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    NetEmrpRelayInfoPkt *pp = (NetEmrpRelayInfoPkt *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(NetEmrpEnergyInfoPkt);

NetEmrpEnergyInfoPkt::NetEmrpEnergyInfoPkt(const char *name, int kind) : ::twsn::NetEmrpPkt(name,kind)
{
    this->setPktSize(23);
    this->setPktType(EMRP_ENERGY_INFO);

    this->remainEnergy_var = 0;
}

NetEmrpEnergyInfoPkt::NetEmrpEnergyInfoPkt(const NetEmrpEnergyInfoPkt& other) : ::twsn::NetEmrpPkt(other)
{
    copy(other);
}

NetEmrpEnergyInfoPkt::~NetEmrpEnergyInfoPkt()
{
}

NetEmrpEnergyInfoPkt& NetEmrpEnergyInfoPkt::operator=(const NetEmrpEnergyInfoPkt& other)
{
    if (this==&other) return *this;
    ::twsn::NetEmrpPkt::operator=(other);
    copy(other);
    return *this;
}

void NetEmrpEnergyInfoPkt::copy(const NetEmrpEnergyInfoPkt& other)
{
    this->remainEnergy_var = other.remainEnergy_var;
}

void NetEmrpEnergyInfoPkt::parsimPack(cCommBuffer *b)
{
    ::twsn::NetEmrpPkt::parsimPack(b);
    doPacking(b,this->remainEnergy_var);
}

void NetEmrpEnergyInfoPkt::parsimUnpack(cCommBuffer *b)
{
    ::twsn::NetEmrpPkt::parsimUnpack(b);
    doUnpacking(b,this->remainEnergy_var);
}

double NetEmrpEnergyInfoPkt::getRemainEnergy() const
{
    return remainEnergy_var;
}

void NetEmrpEnergyInfoPkt::setRemainEnergy(double remainEnergy)
{
    this->remainEnergy_var = remainEnergy;
}

class NetEmrpEnergyInfoPktDescriptor : public cClassDescriptor
{
  public:
    NetEmrpEnergyInfoPktDescriptor();
    virtual ~NetEmrpEnergyInfoPktDescriptor();

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

Register_ClassDescriptor(NetEmrpEnergyInfoPktDescriptor);

NetEmrpEnergyInfoPktDescriptor::NetEmrpEnergyInfoPktDescriptor() : cClassDescriptor("twsn::NetEmrpEnergyInfoPkt", "twsn::NetEmrpPkt")
{
}

NetEmrpEnergyInfoPktDescriptor::~NetEmrpEnergyInfoPktDescriptor()
{
}

bool NetEmrpEnergyInfoPktDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<NetEmrpEnergyInfoPkt *>(obj)!=NULL;
}

const char *NetEmrpEnergyInfoPktDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int NetEmrpEnergyInfoPktDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int NetEmrpEnergyInfoPktDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *NetEmrpEnergyInfoPktDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "remainEnergy",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int NetEmrpEnergyInfoPktDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='r' && strcmp(fieldName, "remainEnergy")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *NetEmrpEnergyInfoPktDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "double",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *NetEmrpEnergyInfoPktDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int NetEmrpEnergyInfoPktDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    NetEmrpEnergyInfoPkt *pp = (NetEmrpEnergyInfoPkt *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string NetEmrpEnergyInfoPktDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    NetEmrpEnergyInfoPkt *pp = (NetEmrpEnergyInfoPkt *)object; (void)pp;
    switch (field) {
        case 0: return double2string(pp->getRemainEnergy());
        default: return "";
    }
}

bool NetEmrpEnergyInfoPktDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    NetEmrpEnergyInfoPkt *pp = (NetEmrpEnergyInfoPkt *)object; (void)pp;
    switch (field) {
        case 0: pp->setRemainEnergy(string2double(value)); return true;
        default: return false;
    }
}

const char *NetEmrpEnergyInfoPktDescriptor::getFieldStructName(void *object, int field) const
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

void *NetEmrpEnergyInfoPktDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    NetEmrpEnergyInfoPkt *pp = (NetEmrpEnergyInfoPkt *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(ResponseRelayInfoTimer);

ResponseRelayInfoTimer::ResponseRelayInfoTimer(const char *name, int kind) : ::cMessage(name,kind)
{
}

ResponseRelayInfoTimer::ResponseRelayInfoTimer(const ResponseRelayInfoTimer& other) : ::cMessage(other)
{
    copy(other);
}

ResponseRelayInfoTimer::~ResponseRelayInfoTimer()
{
}

ResponseRelayInfoTimer& ResponseRelayInfoTimer::operator=(const ResponseRelayInfoTimer& other)
{
    if (this==&other) return *this;
    ::cMessage::operator=(other);
    copy(other);
    return *this;
}

void ResponseRelayInfoTimer::copy(const ResponseRelayInfoTimer& other)
{
    this->reqAddr_var = other.reqAddr_var;
}

void ResponseRelayInfoTimer::parsimPack(cCommBuffer *b)
{
    ::cMessage::parsimPack(b);
    doPacking(b,this->reqAddr_var);
}

void ResponseRelayInfoTimer::parsimUnpack(cCommBuffer *b)
{
    ::cMessage::parsimUnpack(b);
    doUnpacking(b,this->reqAddr_var);
}

twsn::netaddr_t& ResponseRelayInfoTimer::getReqAddr()
{
    return reqAddr_var;
}

void ResponseRelayInfoTimer::setReqAddr(const twsn::netaddr_t& reqAddr)
{
    this->reqAddr_var = reqAddr;
}

class ResponseRelayInfoTimerDescriptor : public cClassDescriptor
{
  public:
    ResponseRelayInfoTimerDescriptor();
    virtual ~ResponseRelayInfoTimerDescriptor();

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

Register_ClassDescriptor(ResponseRelayInfoTimerDescriptor);

ResponseRelayInfoTimerDescriptor::ResponseRelayInfoTimerDescriptor() : cClassDescriptor("twsn::ResponseRelayInfoTimer", "cMessage")
{
}

ResponseRelayInfoTimerDescriptor::~ResponseRelayInfoTimerDescriptor()
{
}

bool ResponseRelayInfoTimerDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<ResponseRelayInfoTimer *>(obj)!=NULL;
}

const char *ResponseRelayInfoTimerDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int ResponseRelayInfoTimerDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int ResponseRelayInfoTimerDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *ResponseRelayInfoTimerDescriptor::getFieldName(void *object, int field) const
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

int ResponseRelayInfoTimerDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='r' && strcmp(fieldName, "reqAddr")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *ResponseRelayInfoTimerDescriptor::getFieldTypeString(void *object, int field) const
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

const char *ResponseRelayInfoTimerDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int ResponseRelayInfoTimerDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    ResponseRelayInfoTimer *pp = (ResponseRelayInfoTimer *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string ResponseRelayInfoTimerDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    ResponseRelayInfoTimer *pp = (ResponseRelayInfoTimer *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getReqAddr(); return out.str();}
        default: return "";
    }
}

bool ResponseRelayInfoTimerDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    ResponseRelayInfoTimer *pp = (ResponseRelayInfoTimer *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *ResponseRelayInfoTimerDescriptor::getFieldStructName(void *object, int field) const
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

void *ResponseRelayInfoTimerDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    ResponseRelayInfoTimer *pp = (ResponseRelayInfoTimer *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getReqAddr()); break;
        default: return NULL;
    }
}

}; // end namespace twsn

