//
// Generated file, do not edit! Created by opp_msgc 4.3 from base/messages/control/command.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "command_m.h"

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
    cEnum *e = cEnum::find("CmdLevel");
    if (!e) enums.getInstance()->add(e = new cEnum("CmdLevel"));
    e->insert(UNKNOWN, "UNKNOWN");
    e->insert(PHYS, "PHYS");
    e->insert(LINK, "LINK");
    e->insert(NETW, "NETW");
    e->insert(TRAN, "TRAN");
    e->insert(APPL, "APPL");
);

EXECUTE_ON_STARTUP(
    cEnum *e = cEnum::find("CmdID");
    if (!e) enums.getInstance()->add(e = new cEnum("CmdID"));
    e->insert(CMD_DATA_NOTI, "CMD_DATA_NOTI");
    e->insert(CMD_DATA_FETCH, "CMD_DATA_FETCH");
    e->insert(CMD_READY, "CMD_READY");
    e->insert(CMD_PHY_PD, "CMD_PHY_PD");
    e->insert(CMD_PHY_IDLE, "CMD_PHY_IDLE");
    e->insert(CMD_PHY_RX, "CMD_PHY_RX");
    e->insert(CMD_PHY_TX, "CMD_PHY_TX");
    e->insert(CMD_PHY_CCA, "CMD_PHY_CCA");
    e->insert(CMD_LIN_CCA_RESULT, "CMD_LIN_CCA_RESULT");
);

Register_Class(Command);

Command::Command(const char *name, int kind) : cMessage(name,kind)
{
    this->src_var = UNKNOWN;
    this->des_var = UNKNOWN;
    this->cmdId_var = 0;
}

Command::Command(const Command& other) : cMessage(other)
{
    copy(other);
}

Command::~Command()
{
}

Command& Command::operator=(const Command& other)
{
    if (this==&other) return *this;
    cMessage::operator=(other);
    copy(other);
    return *this;
}

void Command::copy(const Command& other)
{
    this->src_var = other.src_var;
    this->des_var = other.des_var;
    this->cmdId_var = other.cmdId_var;
}

void Command::parsimPack(cCommBuffer *b)
{
    cMessage::parsimPack(b);
    doPacking(b,this->src_var);
    doPacking(b,this->des_var);
    doPacking(b,this->cmdId_var);
}

void Command::parsimUnpack(cCommBuffer *b)
{
    cMessage::parsimUnpack(b);
    doUnpacking(b,this->src_var);
    doUnpacking(b,this->des_var);
    doUnpacking(b,this->cmdId_var);
}

int Command::getSrc() const
{
    return src_var;
}

void Command::setSrc(int src)
{
    this->src_var = src;
}

int Command::getDes() const
{
    return des_var;
}

void Command::setDes(int des)
{
    this->des_var = des;
}

int Command::getCmdId() const
{
    return cmdId_var;
}

void Command::setCmdId(int cmdId)
{
    this->cmdId_var = cmdId;
}

class CommandDescriptor : public cClassDescriptor
{
  public:
    CommandDescriptor();
    virtual ~CommandDescriptor();

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

Register_ClassDescriptor(CommandDescriptor);

CommandDescriptor::CommandDescriptor() : cClassDescriptor("Command", "cMessage")
{
}

CommandDescriptor::~CommandDescriptor()
{
}

bool CommandDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<Command *>(obj)!=NULL;
}

const char *CommandDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int CommandDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
}

unsigned int CommandDescriptor::getFieldTypeFlags(void *object, int field) const
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
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *CommandDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "src",
        "des",
        "cmdId",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int CommandDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "src")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "des")==0) return base+1;
    if (fieldName[0]=='c' && strcmp(fieldName, "cmdId")==0) return base+2;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *CommandDescriptor::getFieldTypeString(void *object, int field) const
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
        "int",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : NULL;
}

const char *CommandDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int CommandDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    Command *pp = (Command *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string CommandDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    Command *pp = (Command *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getSrc());
        case 1: return long2string(pp->getDes());
        case 2: return long2string(pp->getCmdId());
        default: return "";
    }
}

bool CommandDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    Command *pp = (Command *)object; (void)pp;
    switch (field) {
        case 0: pp->setSrc(string2long(value)); return true;
        case 1: pp->setDes(string2long(value)); return true;
        case 2: pp->setCmdId(string2long(value)); return true;
        default: return false;
    }
}

const char *CommandDescriptor::getFieldStructName(void *object, int field) const
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
    };
    return (field>=0 && field<3) ? fieldStructNames[field] : NULL;
}

void *CommandDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    Command *pp = (Command *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(CmdCCA);

CmdCCA::CmdCCA(const char *name, int kind) : Command(name,kind)
{
    this->setSrc(LINK);
    this->setDes(PHYS);
    this->setCmdId(CMD_PHY_CCA);

    this->duration_var = 0.000128;
}

CmdCCA::CmdCCA(const CmdCCA& other) : Command(other)
{
    copy(other);
}

CmdCCA::~CmdCCA()
{
}

CmdCCA& CmdCCA::operator=(const CmdCCA& other)
{
    if (this==&other) return *this;
    Command::operator=(other);
    copy(other);
    return *this;
}

void CmdCCA::copy(const CmdCCA& other)
{
    this->duration_var = other.duration_var;
}

void CmdCCA::parsimPack(cCommBuffer *b)
{
    Command::parsimPack(b);
    doPacking(b,this->duration_var);
}

void CmdCCA::parsimUnpack(cCommBuffer *b)
{
    Command::parsimUnpack(b);
    doUnpacking(b,this->duration_var);
}

double CmdCCA::getDuration() const
{
    return duration_var;
}

void CmdCCA::setDuration(double duration)
{
    this->duration_var = duration;
}

class CmdCCADescriptor : public cClassDescriptor
{
  public:
    CmdCCADescriptor();
    virtual ~CmdCCADescriptor();

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

Register_ClassDescriptor(CmdCCADescriptor);

CmdCCADescriptor::CmdCCADescriptor() : cClassDescriptor("CmdCCA", "Command")
{
}

CmdCCADescriptor::~CmdCCADescriptor()
{
}

bool CmdCCADescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<CmdCCA *>(obj)!=NULL;
}

const char *CmdCCADescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int CmdCCADescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int CmdCCADescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *CmdCCADescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "duration",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int CmdCCADescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='d' && strcmp(fieldName, "duration")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *CmdCCADescriptor::getFieldTypeString(void *object, int field) const
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

const char *CmdCCADescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int CmdCCADescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    CmdCCA *pp = (CmdCCA *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string CmdCCADescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    CmdCCA *pp = (CmdCCA *)object; (void)pp;
    switch (field) {
        case 0: return double2string(pp->getDuration());
        default: return "";
    }
}

bool CmdCCADescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    CmdCCA *pp = (CmdCCA *)object; (void)pp;
    switch (field) {
        case 0: pp->setDuration(string2double(value)); return true;
        default: return false;
    }
}

const char *CmdCCADescriptor::getFieldStructName(void *object, int field) const
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

void *CmdCCADescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    CmdCCA *pp = (CmdCCA *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}

Register_Class(CmdCCAR);

CmdCCAR::CmdCCAR(const char *name, int kind) : Command(name,kind)
{
    this->setSrc(PHYS);
    this->setDes(LINK);
    this->setCmdId(CMD_LIN_CCA_RESULT);

    this->clearChannel_var = 0;
}

CmdCCAR::CmdCCAR(const CmdCCAR& other) : Command(other)
{
    copy(other);
}

CmdCCAR::~CmdCCAR()
{
}

CmdCCAR& CmdCCAR::operator=(const CmdCCAR& other)
{
    if (this==&other) return *this;
    Command::operator=(other);
    copy(other);
    return *this;
}

void CmdCCAR::copy(const CmdCCAR& other)
{
    this->clearChannel_var = other.clearChannel_var;
}

void CmdCCAR::parsimPack(cCommBuffer *b)
{
    Command::parsimPack(b);
    doPacking(b,this->clearChannel_var);
}

void CmdCCAR::parsimUnpack(cCommBuffer *b)
{
    Command::parsimUnpack(b);
    doUnpacking(b,this->clearChannel_var);
}

bool CmdCCAR::getClearChannel() const
{
    return clearChannel_var;
}

void CmdCCAR::setClearChannel(bool clearChannel)
{
    this->clearChannel_var = clearChannel;
}

class CmdCCARDescriptor : public cClassDescriptor
{
  public:
    CmdCCARDescriptor();
    virtual ~CmdCCARDescriptor();

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

Register_ClassDescriptor(CmdCCARDescriptor);

CmdCCARDescriptor::CmdCCARDescriptor() : cClassDescriptor("CmdCCAR", "Command")
{
}

CmdCCARDescriptor::~CmdCCARDescriptor()
{
}

bool CmdCCARDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<CmdCCAR *>(obj)!=NULL;
}

const char *CmdCCARDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int CmdCCARDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int CmdCCARDescriptor::getFieldTypeFlags(void *object, int field) const
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

const char *CmdCCARDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "clearChannel",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int CmdCCARDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='c' && strcmp(fieldName, "clearChannel")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *CmdCCARDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "bool",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *CmdCCARDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int CmdCCARDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    CmdCCAR *pp = (CmdCCAR *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string CmdCCARDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    CmdCCAR *pp = (CmdCCAR *)object; (void)pp;
    switch (field) {
        case 0: return bool2string(pp->getClearChannel());
        default: return "";
    }
}

bool CmdCCARDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    CmdCCAR *pp = (CmdCCAR *)object; (void)pp;
    switch (field) {
        case 0: pp->setClearChannel(string2bool(value)); return true;
        default: return false;
    }
}

const char *CmdCCARDescriptor::getFieldStructName(void *object, int field) const
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

void *CmdCCARDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    CmdCCAR *pp = (CmdCCAR *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


