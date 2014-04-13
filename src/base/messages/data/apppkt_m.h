//
// Generated file, do not edit! Created by opp_msgc 4.4 from base/messages/data/apppkt.msg.
//

#ifndef _TWSN_APPPKT_M_H_
#define _TWSN_APPPKT_M_H_

#include <omnetpp.h>

// opp_msgc version check
#define MSGC_VERSION 0x0404
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgc: 'make clean' should help.
#endif

// cplusplus {{
#include "twsndef.h"
// }}


namespace twsn {

/**
 * Enum generated from <tt>base/messages/data/apppkt.msg</tt> by opp_msgc.
 * <pre>
 * enum RoutingType {
 *     RT_TO_AN = 0; 
 *     RT_TO_BS = 1; 
 *     RT_BROADCAST = 2; 
 * };
 * </pre>
 */
enum RoutingType {
    RT_TO_AN = 0,
    RT_TO_BS = 1,
    RT_BROADCAST = 2
};

/**
 * Class generated from <tt>base/messages/data/apppkt.msg</tt> by opp_msgc.
 * <pre>
 * packet AppPkt {
 *     
 *     
 *     netaddr_t srcNetAddr; 
 *     netaddr_t desNetAddr; 
 *     int routingType = RT_TO_AN; 
 *     int pktSize = 1; 
 * }
 * </pre>
 */
class AppPkt : public ::cPacket
{
  protected:
    twsn::netaddr_t srcNetAddr_var;
    twsn::netaddr_t desNetAddr_var;
    int routingType_var;
    int pktSize_var;

  private:
    void copy(const AppPkt& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const AppPkt&);

  public:
    AppPkt(const char *name=NULL, int kind=0);
    AppPkt(const AppPkt& other);
    virtual ~AppPkt();
    AppPkt& operator=(const AppPkt& other);
    virtual AppPkt *dup() const {return new AppPkt(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual twsn::netaddr_t& getSrcNetAddr();
    virtual const twsn::netaddr_t& getSrcNetAddr() const {return const_cast<AppPkt*>(this)->getSrcNetAddr();}
    virtual void setSrcNetAddr(const twsn::netaddr_t& srcNetAddr);
    virtual twsn::netaddr_t& getDesNetAddr();
    virtual const twsn::netaddr_t& getDesNetAddr() const {return const_cast<AppPkt*>(this)->getDesNetAddr();}
    virtual void setDesNetAddr(const twsn::netaddr_t& desNetAddr);
    virtual int getRoutingType() const;
    virtual void setRoutingType(int routingType);
    virtual int getPktSize() const;
    virtual void setPktSize(int pktSize);
};

inline void doPacking(cCommBuffer *b, AppPkt& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, AppPkt& obj) {obj.parsimUnpack(b);}

}; // end namespace twsn

#endif // _TWSN_APPPKT_M_H_
