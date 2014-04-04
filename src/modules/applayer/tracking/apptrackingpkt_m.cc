//
// Generated file, do not edit! Created by opp_msgc 4.3 from modules/applayer/tracking/apptrackingpkt.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "apptrackingpkt_m.h"

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
    cEnum *e = cEnum::find("twsn::AppTrackingPktType");
    if (!e) enums.getInstance()->add(e = new cEnum("twsn::AppTrackingPktType"));
    e->insert(AT_SYNC_REQUEST, "AT_SYNC_REQUEST");
    e->insert(AT_MEASUREMENT, "AT_MEASUREMENT");
    e->insert(AT_TARGET_POSITION, "AT_TARGET_POSITION");
    e->insert(AT_CH_BEACON, "AT_CH_BEACON");
);

Register_Class(AppTrackingPkt);

AppTrackingPkt::AppTrackingPkt(const char *name, int kind) : twsn::AppPkt(name,kind)
{
    this->setPktSize(2);

    this->pktType_var = 0;
}

AppTrackingPkt::AppTrackingPkt(const AppTrackingPkt& other) : twsn::AppPkt(other)
{
    copy(other);
}

AppTrackingPkt::~AppTrackingPkt()
{
}

AppTrackingPkt& AppTrackingPkt::operator=(const AppTrackingPkt& other)
{
    if (this==&other) return *this;
    twsn::AppPkt::operator=(other);
    copy(other);
    return *this;
}

void AppTrackingPkt::copy(const AppTrackingPkt& other)
{
    this->pktType_var = other.pktType_var;
}

void AppTrackingPkt::parsimPack(cCommBuffer *b)
{
    twsn::AppPkt::parsimPack(b);
    doPacking(b,this->pktType_var);
}

void AppTrackingPkt::parsimUnpack(cCommBuffer *b)
{
    twsn::AppPkt::parsimUnpack(b);
    doUnpacking(b,this->pktType_var);
}

int AppTrackingPkt::getPktType() const
{
    return pktType_var;
}

void AppTrackingPkt::setPktType(int pktType)
{
    this->pktType_var = pktType;
}

class AppTrackingPktDescriptor : public cClassDescriptor
{
  public:
    AppTrackingPktDescriptor();
    virtual ~AppTrackingPktDescriptor();

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

Register_ClassDescriptor(AppTrackingPktDescriptor);

AppTrackingPktDescriptor::AppTrackingPktDescriptor() : cClassDescriptor("twsn::AppTrackingPkt", "twsn::AppPkt")
{
}

AppTrackingPktDescriptor::~AppTrackingPktDescriptor()
{
}

bool AppTrackingPktDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<AppTrackingPkt *>(obj)!=NULL;
}

const char *AppTrackingPktDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int AppTrackingPktDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int AppTrackingPktDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *AppTrackingPktDescriptor::getFieldName(void *object, int field) const
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

int AppTrackingPktDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "pktType")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *AppTrackingPktDescriptor::getFieldTypeString(void *object, int field) const
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

const char *AppTrackingPktDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int AppTrackingPktDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    AppTrackingPkt *pp = (AppTrackingPkt *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string AppTrackingPktDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    AppTrackingPkt *pp = (AppTrackingPkt *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getPktType());
        default: return "";
    }
}

bool AppTrackingPktDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    AppTrackingPkt *pp = (AppTrackingPkt *)object; (void)pp;
    switch (field) {
        case 0: pp->setPktType(string2long(value)); return true;
        default: return false;
    }
}

const char *AppTrackingPktDescriptor::getFieldStructName(void *object, int field) const
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

void *AppTrackingPktDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    AppTrackingPkt *pp = (AppTrackingPkt *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(AT_MeasurementPkt);

AT_MeasurementPkt::AT_MeasurementPkt(const char *name, int kind) : twsn::AppTrackingPkt(name,kind)
{
    this->setRoutingType(RT_BROADCAST);
    this->setPktType(AT_MEASUREMENT);
    this->setPktSize(18);

}

AT_MeasurementPkt::AT_MeasurementPkt(const AT_MeasurementPkt& other) : twsn::AppTrackingPkt(other)
{
    copy(other);
}

AT_MeasurementPkt::~AT_MeasurementPkt()
{
}

AT_MeasurementPkt& AT_MeasurementPkt::operator=(const AT_MeasurementPkt& other)
{
    if (this==&other) return *this;
    twsn::AppTrackingPkt::operator=(other);
    copy(other);
    return *this;
}

void AT_MeasurementPkt::copy(const AT_MeasurementPkt& other)
{
    this->measurement_var = other.measurement_var;
}

void AT_MeasurementPkt::parsimPack(cCommBuffer *b)
{
    twsn::AppTrackingPkt::parsimPack(b);
    doPacking(b,this->measurement_var);
}

void AT_MeasurementPkt::parsimUnpack(cCommBuffer *b)
{
    twsn::AppTrackingPkt::parsimUnpack(b);
    doUnpacking(b,this->measurement_var);
}

twsn::Measurement& AT_MeasurementPkt::getMeasurement()
{
    return measurement_var;
}

void AT_MeasurementPkt::setMeasurement(const twsn::Measurement& measurement)
{
    this->measurement_var = measurement;
}

class AT_MeasurementPktDescriptor : public cClassDescriptor
{
  public:
    AT_MeasurementPktDescriptor();
    virtual ~AT_MeasurementPktDescriptor();

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

Register_ClassDescriptor(AT_MeasurementPktDescriptor);

AT_MeasurementPktDescriptor::AT_MeasurementPktDescriptor() : cClassDescriptor("twsn::AT_MeasurementPkt", "twsn::AppTrackingPkt")
{
}

AT_MeasurementPktDescriptor::~AT_MeasurementPktDescriptor()
{
}

bool AT_MeasurementPktDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<AT_MeasurementPkt *>(obj)!=NULL;
}

const char *AT_MeasurementPktDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int AT_MeasurementPktDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int AT_MeasurementPktDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *AT_MeasurementPktDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "measurement",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int AT_MeasurementPktDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='m' && strcmp(fieldName, "measurement")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *AT_MeasurementPktDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "Measurement",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *AT_MeasurementPktDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int AT_MeasurementPktDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    AT_MeasurementPkt *pp = (AT_MeasurementPkt *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string AT_MeasurementPktDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    AT_MeasurementPkt *pp = (AT_MeasurementPkt *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getMeasurement(); return out.str();}
        default: return "";
    }
}

bool AT_MeasurementPktDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    AT_MeasurementPkt *pp = (AT_MeasurementPkt *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *AT_MeasurementPktDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        "twsn::Measurement",
    };
    return (field>=0 && field<1) ? fieldStructNames[field] : NULL;
}

void *AT_MeasurementPktDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    AT_MeasurementPkt *pp = (AT_MeasurementPkt *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getMeasurement()); break;
        default: return NULL;
    }
}

Register_Class(AT_TargetPosPkt);

AT_TargetPosPkt::AT_TargetPosPkt(const char *name, int kind) : twsn::AppTrackingPkt(name,kind)
{
    this->setRoutingType(RT_TO_BS);
    this->setPktType(AT_TARGET_POSITION);
    this->setPktSize(19);

}

AT_TargetPosPkt::AT_TargetPosPkt(const AT_TargetPosPkt& other) : twsn::AppTrackingPkt(other)
{
    copy(other);
}

AT_TargetPosPkt::~AT_TargetPosPkt()
{
}

AT_TargetPosPkt& AT_TargetPosPkt::operator=(const AT_TargetPosPkt& other)
{
    if (this==&other) return *this;
    twsn::AppTrackingPkt::operator=(other);
    copy(other);
    return *this;
}

void AT_TargetPosPkt::copy(const AT_TargetPosPkt& other)
{
    this->targetPos_var = other.targetPos_var;
}

void AT_TargetPosPkt::parsimPack(cCommBuffer *b)
{
    twsn::AppTrackingPkt::parsimPack(b);
    doPacking(b,this->targetPos_var);
}

void AT_TargetPosPkt::parsimUnpack(cCommBuffer *b)
{
    twsn::AppTrackingPkt::parsimUnpack(b);
    doUnpacking(b,this->targetPos_var);
}

twsn::TargetPos& AT_TargetPosPkt::getTargetPos()
{
    return targetPos_var;
}

void AT_TargetPosPkt::setTargetPos(const twsn::TargetPos& targetPos)
{
    this->targetPos_var = targetPos;
}

class AT_TargetPosPktDescriptor : public cClassDescriptor
{
  public:
    AT_TargetPosPktDescriptor();
    virtual ~AT_TargetPosPktDescriptor();

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

Register_ClassDescriptor(AT_TargetPosPktDescriptor);

AT_TargetPosPktDescriptor::AT_TargetPosPktDescriptor() : cClassDescriptor("twsn::AT_TargetPosPkt", "twsn::AppTrackingPkt")
{
}

AT_TargetPosPktDescriptor::~AT_TargetPosPktDescriptor()
{
}

bool AT_TargetPosPktDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<AT_TargetPosPkt *>(obj)!=NULL;
}

const char *AT_TargetPosPktDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int AT_TargetPosPktDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int AT_TargetPosPktDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *AT_TargetPosPktDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "targetPos",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int AT_TargetPosPktDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='t' && strcmp(fieldName, "targetPos")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *AT_TargetPosPktDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "TargetPos",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *AT_TargetPosPktDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int AT_TargetPosPktDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    AT_TargetPosPkt *pp = (AT_TargetPosPkt *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string AT_TargetPosPktDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    AT_TargetPosPkt *pp = (AT_TargetPosPkt *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getTargetPos(); return out.str();}
        default: return "";
    }
}

bool AT_TargetPosPktDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    AT_TargetPosPkt *pp = (AT_TargetPosPkt *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *AT_TargetPosPktDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        "twsn::TargetPos",
    };
    return (field>=0 && field<1) ? fieldStructNames[field] : NULL;
}

void *AT_TargetPosPktDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    AT_TargetPosPkt *pp = (AT_TargetPosPkt *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getTargetPos()); break;
        default: return NULL;
    }
}

}; // end namespace twsn

