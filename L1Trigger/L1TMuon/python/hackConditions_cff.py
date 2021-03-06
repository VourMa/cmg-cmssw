#
# hackConditions.py  Load ES Producers for any conditions not yet in GT...
#
# The intention is that this file should shrink with time as conditions are added to GT.
#

import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras

#
# Legacy Trigger:  No Hacks Needed
#
#if not (eras.stage1L1Trigger.isChosen() or eras.stage2L1Trigger.isChosen()):
#    print "L1TMuon conditions configured for Run1 (Legacy) trigger. "
# 

#
# Stage-1 Trigger:  No Hacks Needed
#
#if eras.stage1L1Trigger.isChosen() and not eras.stage2L1Trigger.isChosen():
#    print "L1TMuon Conditions configured for Stage-1 (2015) trigger. "    

#
# Stage-2 Trigger
#
if eras.stage2L1Trigger.isChosen():
    print "L1TMuon Conditions configured for Stage-2 (2016) trigger. "
    from L1Trigger.L1TMuonBarrel.fakeBmtfParams_cff import *
    from L1Trigger.L1TMuonOverlap.fakeOmtfParams_cff import *
    from L1Trigger.L1TMuonEndCap.fakeEmtfParams_cff import *
    from L1Trigger.L1TMuon.fakeGmtParams_cff import *
