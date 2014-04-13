//
// Generated file, do not edit! Created by opp_msgc 4.4 from base/messages/data/macpkt.msg.
//

#ifndef _TWSN_MACPKT_M_H_
#define _TWSN_MACPKT_M_H_

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
 * Class generated from <tt>base/messages/data/macpkt.msg</tt> by opp_msgc.
 * <pre>
 * packet MacPkt {
 *     macaddr_t srcAddr; 
 *     macaddr_t desAddr; 
 *     int pktSize = 16; 
 * }
 * </pre>
 */
class MacPkt : public ::cPacket
{
  protected:
    twsn::macaddr_t srcAddr_var;
    twsn::macaddr_t desAddr_var;
    int pktSize_var;

  private:
    void copy(const MacPkt& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const MacPkt&);

  public:
    MacPkt(const char *name=NULL, int kind=0);
    MacPkt(const MacPkt& other);
    virtual ~MacPkt();
    MacPkt& operator=(const MacPkt& other);
    virtual MacPkt *dup() const {return new MacPkt(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual twsn::macaddr_t& getSrcAddr();
    virtual const twsn::macaddr_t& getSrcAddr() const {return const_cast<MacPkt*>(this)->getSrcAddr();}
    virtual void setSrcAddr(const twsn::macaddr_t& srcAddr);
    virtual twsn::macaddr_t& getDesAddr();
    virtual const twsn::macaddr_t& getDesAddr() const {return const_cast<MacPkt*>(this)->getDesAddr();}
    virtual void setDesAddr(const twsn::macaddr_t& desAddr);
    virtual int getPktSize() const;
    virtual void setPktSize(int pktSize);
};

inline void doPacking(cCommBuffer *b, MacPkt& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, MacPkt& obj) {obj.parsimUnpack(b);}

}; // end namespace twsn

#endif // _TWSN_MACPKT_M_H_
