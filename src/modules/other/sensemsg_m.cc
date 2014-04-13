//
// Generated file, do not edit! Created by opp_msgc 4.4 from modules/other/sensemsg.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "sensemsg_m.h"

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
    cEnum *e = cEnum::find("twsn::SenseMsgType");
    if (!e) enums.getInstance()->add(e = new cEnum("twsn::SenseMsgType"));
    e->insert(SS_START, "SS_START");
    e->insert(SS_CANCEL, "SS_CANCEL");
    e->insert(SS_RESULT, "SS_RESULT");
    e->insert(SS_ACTION, "SS_ACTION");
    e->insert(SS_SIGNAL, "SS_SIGNAL");
);

Register_Class(SenseMsg);

SenseMsg::SenseMsg(const char *name, int kind) : ::cMessage(name,kind)
{
    this->msgType_var = 0;
}

SenseMsg::SenseMsg(const SenseMsg& other) : ::cMessage(other)
{
    copy(other);
}

SenseMsg::~SenseMsg()
{
}

SenseMsg& SenseMsg::operator=(const SenseMsg& other)
{
    if (this==&other) return *this;
    ::cMessage::operator=(other);
    copy(other);
    return *this;
}

void SenseMsg::copy(const SenseMsg& other)
{
    this->msgType_var = other.msgType_var;
}

void SenseMsg::parsimPack(cCommBuffer *b)
{
    ::cMessage::parsimPack(b);
    doPacking(b,this->msgType_var);
}

void SenseMsg::parsimUnpack(cCommBuffer *b)
{
    ::cMessage::parsimUnpack(b);
    doUnpacking(b,this->msgType_var);
}

int SenseMsg::getMsgType() const
{
    return msgType_var;
}

void SenseMsg::setMsgType(int msgType)
{
    this->msgType_var = msgType;
}

class SenseMsgDescriptor : public cClassDescriptor
{
  public:
    SenseMsgDescriptor();
    virtual ~SenseMsgDescriptor();

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

Register_ClassDescriptor(SenseMsgDescriptor);

SenseMsgDescriptor::SenseMsgDescriptor() : cClassDescriptor("twsn::SenseMsg", "cMessage")
{
}

SenseMsgDescriptor::~SenseMsgDescriptor()
{
}

bool SenseMsgDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<SenseMsg *>(obj)!=NULL;
}

const char *SenseMsgDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int SenseMsgDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int SenseMsgDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *SenseMsgDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "msgType",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int SenseMsgDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='m' && strcmp(fieldName, "msgType")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *SenseMsgDescriptor::getFieldTypeString(void *object, int field) const
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

const char *SenseMsgDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int SenseMsgDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    SenseMsg *pp = (SenseMsg *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string SenseMsgDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    SenseMsg *pp = (SenseMsg *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getMsgType());
        default: return "";
    }
}

bool SenseMsgDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    SenseMsg *pp = (SenseMsg *)object; (void)pp;
    switch (field) {
        case 0: pp->setMsgType(string2long(value)); return true;
        default: return false;
    }
}

const char *SenseMsgDescriptor::getFieldStructName(void *object, int field) const
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

void *SenseMsgDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    SenseMsg *pp = (SenseMsg *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(SenseResult);

SenseResult::SenseResult(const char *name, int kind) : ::twsn::SenseMsg(name,kind)
{
    this->setMsgType(SS_RESULT);

}

SenseResult::SenseResult(const SenseResult& other) : ::twsn::SenseMsg(other)
{
    copy(other);
}

SenseResult::~SenseResult()
{
}

SenseResult& SenseResult::operator=(const SenseResult& other)
{
    if (this==&other) return *this;
    ::twsn::SenseMsg::operator=(other);
    copy(other);
    return *this;
}

void SenseResult::copy(const SenseResult& other)
{
    this->meaList_var = other.meaList_var;
}

void SenseResult::parsimPack(cCommBuffer *b)
{
    ::twsn::SenseMsg::parsimPack(b);
    doPacking(b,this->meaList_var);
}

void SenseResult::parsimUnpack(cCommBuffer *b)
{
    ::twsn::SenseMsg::parsimUnpack(b);
    doUnpacking(b,this->meaList_var);
}

twsn::MeasurementList& SenseResult::getMeaList()
{
    return meaList_var;
}

void SenseResult::setMeaList(const twsn::MeasurementList& meaList)
{
    this->meaList_var = meaList;
}

class SenseResultDescriptor : public cClassDescriptor
{
  public:
    SenseResultDescriptor();
    virtual ~SenseResultDescriptor();

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

Register_ClassDescriptor(SenseResultDescriptor);

SenseResultDescriptor::SenseResultDescriptor() : cClassDescriptor("twsn::SenseResult", "twsn::SenseMsg")
{
}

SenseResultDescriptor::~SenseResultDescriptor()
{
}

bool SenseResultDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<SenseResult *>(obj)!=NULL;
}

const char *SenseResultDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int SenseResultDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int SenseResultDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *SenseResultDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "meaList",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int SenseResultDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='m' && strcmp(fieldName, "meaList")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *SenseResultDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "MeasurementList",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *SenseResultDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int SenseResultDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    SenseResult *pp = (SenseResult *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string SenseResultDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    SenseResult *pp = (SenseResult *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getMeaList(); return out.str();}
        default: return "";
    }
}

bool SenseResultDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    SenseResult *pp = (SenseResult *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *SenseResultDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        "twsn::MeasurementList",
    };
    return (field>=0 && field<1) ? fieldStructNames[field] : NULL;
}

void *SenseResultDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    SenseResult *pp = (SenseResult *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getMeaList()); break;
        default: return NULL;
    }
}

Register_Class(SenseAction);

SenseAction::SenseAction(const char *name, int kind) : ::twsn::SenseMsg(name,kind)
{
    this->setMsgType(SS_ACTION);

    this->sensorId_var = 0;
    this->mobilityId_var = 0;
}

SenseAction::SenseAction(const SenseAction& other) : ::twsn::SenseMsg(other)
{
    copy(other);
}

SenseAction::~SenseAction()
{
}

SenseAction& SenseAction::operator=(const SenseAction& other)
{
    if (this==&other) return *this;
    ::twsn::SenseMsg::operator=(other);
    copy(other);
    return *this;
}

void SenseAction::copy(const SenseAction& other)
{
    this->sensorId_var = other.sensorId_var;
    this->mobilityId_var = other.mobilityId_var;
}

void SenseAction::parsimPack(cCommBuffer *b)
{
    ::twsn::SenseMsg::parsimPack(b);
    doPacking(b,this->sensorId_var);
    doPacking(b,this->mobilityId_var);
}

void SenseAction::parsimUnpack(cCommBuffer *b)
{
    ::twsn::SenseMsg::parsimUnpack(b);
    doUnpacking(b,this->sensorId_var);
    doUnpacking(b,this->mobilityId_var);
}

int SenseAction::getSensorId() const
{
    return sensorId_var;
}

void SenseAction::setSensorId(int sensorId)
{
    this->sensorId_var = sensorId;
}

int SenseAction::getMobilityId() const
{
    return mobilityId_var;
}

void SenseAction::setMobilityId(int mobilityId)
{
    this->mobilityId_var = mobilityId;
}

class SenseActionDescriptor : public cClassDescriptor
{
  public:
    SenseActionDescriptor();
    virtual ~SenseActionDescriptor();

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

Register_ClassDescriptor(SenseActionDescriptor);

SenseActionDescriptor::SenseActionDescriptor() : cClassDescriptor("twsn::SenseAction", "twsn::SenseMsg")
{
}

SenseActionDescriptor::~SenseActionDescriptor()
{
}

bool SenseActionDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<SenseAction *>(obj)!=NULL;
}

const char *SenseActionDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int SenseActionDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int SenseActionDescriptor::getFieldTypeFlags(void *object, int field) const
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
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *SenseActionDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "sensorId",
        "mobilityId",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int SenseActionDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "sensorId")==0) return base+0;
    if (fieldName[0]=='m' && strcmp(fieldName, "mobilityId")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *SenseActionDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *SenseActionDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int SenseActionDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    SenseAction *pp = (SenseAction *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string SenseActionDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    SenseAction *pp = (SenseAction *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getSensorId());
        case 1: return long2string(pp->getMobilityId());
        default: return "";
    }
}

bool SenseActionDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    SenseAction *pp = (SenseAction *)object; (void)pp;
    switch (field) {
        case 0: pp->setSensorId(string2long(value)); return true;
        case 1: pp->setMobilityId(string2long(value)); return true;
        default: return false;
    }
}

const char *SenseActionDescriptor::getFieldStructName(void *object, int field) const
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
    };
    return (field>=0 && field<2) ? fieldStructNames[field] : NULL;
}

void *SenseActionDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    SenseAction *pp = (SenseAction *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(SenseSignal);

SenseSignal::SenseSignal(const char *name, int kind) : ::twsn::SenseMsg(name,kind)
{
    this->setMsgType(SS_SIGNAL);

    this->tarId_var = 0;
    this->distance_var = 0;
    this->x_var = 0;
    this->y_var = 0;
}

SenseSignal::SenseSignal(const SenseSignal& other) : ::twsn::SenseMsg(other)
{
    copy(other);
}

SenseSignal::~SenseSignal()
{
}

SenseSignal& SenseSignal::operator=(const SenseSignal& other)
{
    if (this==&other) return *this;
    ::twsn::SenseMsg::operator=(other);
    copy(other);
    return *this;
}

void SenseSignal::copy(const SenseSignal& other)
{
    this->tarId_var = other.tarId_var;
    this->distance_var = other.distance_var;
    this->x_var = other.x_var;
    this->y_var = other.y_var;
}

void SenseSignal::parsimPack(cCommBuffer *b)
{
    ::twsn::SenseMsg::parsimPack(b);
    doPacking(b,this->tarId_var);
    doPacking(b,this->distance_var);
    doPacking(b,this->x_var);
    doPacking(b,this->y_var);
}

void SenseSignal::parsimUnpack(cCommBuffer *b)
{
    ::twsn::SenseMsg::parsimUnpack(b);
    doUnpacking(b,this->tarId_var);
    doUnpacking(b,this->distance_var);
    doUnpacking(b,this->x_var);
    doUnpacking(b,this->y_var);
}

int SenseSignal::getTarId() const
{
    return tarId_var;
}

void SenseSignal::setTarId(int tarId)
{
    this->tarId_var = tarId;
}

double SenseSignal::getDistance() const
{
    return distance_var;
}

void SenseSignal::setDistance(double distance)
{
    this->distance_var = distance;
}

double SenseSignal::getX() const
{
    return x_var;
}

void SenseSignal::setX(double x)
{
    this->x_var = x;
}

double SenseSignal::getY() const
{
    return y_var;
}

void SenseSignal::setY(double y)
{
    this->y_var = y;
}

class SenseSignalDescriptor : public cClassDescriptor
{
  public:
    SenseSignalDescriptor();
    virtual ~SenseSignalDescriptor();

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

Register_ClassDescriptor(SenseSignalDescriptor);

SenseSignalDescriptor::SenseSignalDescriptor() : cClassDescriptor("twsn::SenseSignal", "twsn::SenseMsg")
{
}

SenseSignalDescriptor::~SenseSignalDescriptor()
{
}

bool SenseSignalDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<SenseSignal *>(obj)!=NULL;
}

const char *SenseSignalDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int SenseSignalDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount(object) : 4;
}

unsigned int SenseSignalDescriptor::getFieldTypeFlags(void *object, int field) const
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
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *SenseSignalDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "tarId",
        "distance",
        "x",
        "y",
    };
    return (field>=0 && field<4) ? fieldNames[field] : NULL;
}

int SenseSignalDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='t' && strcmp(fieldName, "tarId")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "distance")==0) return base+1;
    if (fieldName[0]=='x' && strcmp(fieldName, "x")==0) return base+2;
    if (fieldName[0]=='y' && strcmp(fieldName, "y")==0) return base+3;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *SenseSignalDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "double",
        "double",
        "double",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : NULL;
}

const char *SenseSignalDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int SenseSignalDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    SenseSignal *pp = (SenseSignal *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string SenseSignalDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    SenseSignal *pp = (SenseSignal *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getTarId());
        case 1: return double2string(pp->getDistance());
        case 2: return double2string(pp->getX());
        case 3: return double2string(pp->getY());
        default: return "";
    }
}

bool SenseSignalDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    SenseSignal *pp = (SenseSignal *)object; (void)pp;
    switch (field) {
        case 0: pp->setTarId(string2long(value)); return true;
        case 1: pp->setDistance(string2double(value)); return true;
        case 2: pp->setX(string2double(value)); return true;
        case 3: pp->setY(string2double(value)); return true;
        default: return false;
    }
}

const char *SenseSignalDescriptor::getFieldStructName(void *object, int field) const
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
    };
    return (field>=0 && field<4) ? fieldStructNames[field] : NULL;
}

void *SenseSignalDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    SenseSignal *pp = (SenseSignal *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

}; // end namespace twsn

