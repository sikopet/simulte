//
//                           SimuLTE
//
// This file is part of a software released under the license included in file
// "license.pdf". This license can be also found at http://www.ltesimulator.com/
// The above file and the present reference are part of the software itself,
// and cannot be removed from it.
//

#ifndef LTE_D2DMODESELECTIONBASE_H_
#define LTE_D2DMODESELECTIONBASE_H_

#include "stack/mac/layer/LteMacEnb.h"

//
// D2DModeSelectionBase
// Base class for D2D Mode Selection modules
// To add a new policy for mode selection, extend this class and redefine pure virtual methods
//
class D2DModeSelectionBase : public cSimpleModule
{

protected:

    typedef std::pair<MacNodeId, MacNodeId> FlowId;
    typedef struct
    {
        FlowId flow;
        LteD2DMode oldMode;
        LteD2DMode newMode;
    } FlowModeInfo;
    typedef std::list<FlowModeInfo> SwitchList;
    SwitchList switchList_;  // a list of pairs of nodeIds, where the first node represents the transmitter
                             // of the flow, whereas the second node represents the receiver

    // for each D2D-capable UE, store the list of possible D2D peers and the corresponding communication mode (IM or DM)
    std::map<MacNodeId, std::map<MacNodeId, LteD2DMode> >* peeringModeMap_;

    // reference to the MAC layer
    LteMacEnb* mac_;

    // reference to the binder
    LteBinder* binder_;

    // period between two selection instances
    double modeSelectionPeriod_;

    // Self message
    cMessage* modeSelectionTick_;

    // run the mode selection algorithm. To be implemented by derived classes
    // it must build a switch list (see above)
    virtual void doModeSelection() = 0;

    // for each pair of UEs in the switch list, send the notification to do the
    // switch to the transmitter UE
    void sendModeSwitchNotifications();

public:
    D2DModeSelectionBase() {}
    virtual ~D2DModeSelectionBase() {}

    virtual void initialize(int stage);
    virtual int numInitStages() const { return inet::NUM_INIT_STAGES; }
    virtual void handleMessage(cMessage *msg);

    void doModeSwitchAtHandover(MacNodeId nodeId);
};

#endif /* LTE_D2DMODESELECTIONBASE_H_ */
