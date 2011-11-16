/**
 * @class CalEnergySelectionTool
 *
 * @brief Defines an interfact to a tool which will "select" the best energy from the
 *        available choices
 *
 * @author The Tracking Software Group
 *
 * $Header: /nfs/slac/g/glast/ground/cvs/TkrRecon/src/Track/CalEnergySelectionTool.h,v 1.6 2005/02/11 07:14:52 lsrea Exp $
 */
#ifndef ICalEnergySelectionTool_h
#define ICalEnergySelectionTool_h

#include "GaudiKernel/IAlgTool.h"

namespace Event
{
    class CalCorToolResult;
    class CalEventEnergy;
    class TreeClusterRelation;
};

static const InterfaceID IID_ICalEnergySelectionTool("ICalEnergySelectionTool", 1 , 0);

class ICalEnergySelectionTool : virtual public IAlgTool
{
public:

    /// @brief Defines a method use to set the energies for the tracks before the
    ///        track fit is run
    virtual const Event::CalCorToolResult* selectBestEnergy(Event::CalEventEnergy*      calEnergy,
                                                            Event::TreeClusterRelation* treeClusRel) = 0;

    /// Retrieve interface ID
    static const InterfaceID& interfaceID() { return IID_ICalEnergySelectionTool; }
};

#endif
