//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "apptrackingbs.h"
#include "apptrackingpkt_m.h"
#include <fstream>

namespace twsn {

Define_Module(AppTrackingBS);

void AppTrackingBS::processTarPos(TargetPos &tp)
{
    if (traceList.size() == 0) {
        // Add new trace with this TargetPos
        TargetTrace trace(par("theta").doubleValue(), par("minDeltaT").doubleValue());
        trace.setId(numTrace);
        trace.addTargetPos(tp);
        traceList.push_back(trace);
        numTrace++;
        std::cerr << "First trace, ID " << trace.getId() << endl;
    } else {
        // Add TargetPos to most likely trace
        double mostLikely = -1; // Unacceptable
        TargetTrace *mostLikelyTrace = NULL;
        double likely;

        int id = 0;
        int mostLikelyId = 0;
        for (std::list<TargetTrace>::iterator it = traceList.begin(); it != traceList.end(); it++) {
            likely = (*it).checkLikelihood(tp, par("distanceThreshold").doubleValue(),
                    par("timeThreshold").doubleValue());
            if (likely >= 0) {
                if (mostLikely < 0 || likely < mostLikely) {
                    mostLikely = likely;
                    mostLikelyTrace = &(*it);
                    mostLikelyId = id;
                }
            }
            id++;
        }

        if (mostLikelyTrace != NULL) {
            mostLikelyTrace->addTargetPos(tp);
            std::cerr << "Add to trace ID " << mostLikelyId << endl;
        } else {
            // Add new trace with this TargetPos
            TargetTrace trace(par("theta").doubleValue(), par("minDeltaT").doubleValue());
            trace.setId(numTrace);
            trace.addTargetPos(tp);
            traceList.push_back(trace);
            numTrace++;
            std::cerr << "New trace, ID " << trace.getId() << endl;
        }
    }
}

void AppTrackingBS::cleanJunk()
{
    std::list<TargetTrace>::iterator ttIt = traceList.begin();

    while (ttIt != traceList.end()) {
        double lastTimestamp = (*ttIt).getPath().back().getTimestamp();

        if (simTime() - lastTimestamp > par("minJunkTraceOld").doubleValue()
                && (*ttIt).getPathLen() <= par("maxJunkTraceLen").longValue()) {
            std::cerr << "Cleaning trace ID " << (*ttIt).getId() << endl;
            ttIt = traceList.erase(ttIt);
        } else {
            ttIt++;
        }
    }
}

void AppTrackingBS::output()
{
    cConfigurationEx *configEx = ev.getConfigEx();
    std::ostringstream oss;
    std::ofstream out;
    std::list<TargetTrace>::iterator ttIt;
    std::list<TargetPos> path;
    std::list<TargetPos>::iterator tpIt;

    for (ttIt = traceList.begin(); ttIt != traceList.end(); ttIt++) {
        oss.seekp(0);
        oss << "results/bs_output/";
        oss << configEx->getActiveConfigName() << "_trace_" << (*ttIt).getId() << ".data\0";

        out.open(oss.str().c_str(), std::ios::out | std::ios::trunc);
        if (!out) {
            std::cerr << "Cannot open file " << oss.str() << endl;
        } else {
            out << "# Config: " << configEx->getActiveConfigName() << endl;
            out << "# Trace ID " << (*ttIt).getId() << endl;
            out << "# x y" << endl;

            path = (*ttIt).getPath();
            for (tpIt = path.begin(); tpIt != path.end(); tpIt++) {
                double x = (*tpIt).getCoord().getX();
                double y = (*tpIt).getCoord().getY();
                out << x << ' ' << y << endl;
            }
            out.close();
        }
    }
}

void AppTrackingBS::initialize()
{
    numTrace = 0;
    sigEtoEDelay = registerSignal("sigEtoEDelay");

    /* Clear output folder */
#if defined(__linux__) || defined(__unix__)
    system("exec rm ./results/bs_output/*");
#else
    #if defined(_WIN32)
        system("del .\results\bs_output\*");
    #endif
#endif
}

void AppTrackingBS::handleLowerMsg(cMessage* msg)
{
    AppTrackingPkt *pkt = check_and_cast<AppTrackingPkt*>(msg);

    if (pkt->getPktType() == AT_TARGET_POSITION) {
        AT_TargetPosPkt *tpPkt = check_and_cast<AT_TargetPosPkt*>(msg);
        TargetPos tp = tpPkt->getTargetPos();

        // Record end-to-end delay
        emit(sigEtoEDelay, simTime() - tp.getTimestamp());

        // Process data
        processTarPos(tp);
        cleanJunk();
    }
    delete msg;
}

void AppTrackingBS::finish()
{
    // Clean junk without considering old
    std::list<TargetTrace>::iterator ttIt = traceList.begin();
    while (ttIt != traceList.end()) {
        if ((*ttIt).getPathLen() <= par("maxJunkTraceLen").longValue()) {
            std::cerr << "Cleaning trace ID " << (*ttIt).getId() << endl;
            ttIt = traceList.erase(ttIt);
        } else {
            ttIt++;
        }
    }

    output();
}

}  // namespace twsn
