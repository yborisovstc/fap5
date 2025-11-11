MyRoot : Elem {
    + DesUtils
    # "CP of object of ordering"
    Launcher : DesLauncher {
        LogLevel = "Dbg"
        StInp : State {
            = "SB true"
        }
        Chg : DesUtils.BChange (
            SInp ~ StInp
        )
        BChange_Dbg : State (
            _@ <  {
                = "SB false"
                LogLevel = "Dbg"
            }
            Inp ~ Chg.Outp
        )
        BChange_Cnt : DesUtils.BChangeCnt (
            SInp ~ StInp
        )
        # "Data pulse"
        SDPulse : State (
            _@ <  {
                LogLevel = "Dbg"
                = "URI"
            }
            Dp : DesUtils.DPulse (
                InpD ~ : State {
                    = "URI Hello.World"
                }
                InpE ~ : State {
                    = "URI"
                }
            )
            Dp.Delay < = "URI"
            Inp ~ Dp.Outp
        )
        # "Vector iterator"
        SVect : State {
            = "VDU (URI a1.a2 , URI b1.b2 , URI c1.c2 )"
        }
        VectIter : DesUtils.IdxItr (
            _@ < LogLevel = "Dbg"
            Sw1 < LogLevel = "Dbg"
            InpCnt ~ : TrSizeVar (
                Inp ~ SVect
            )
            InpDone ~ : State {
                = "SB true"
            }
            InpReset ~ : State {
                = "SB false"
            }
        )
        VectIter_Dbg : State (
            Inp ~ VectIter.Outp
        )
        VectIter_Done_Dbg : State (
            Inp ~ VectIter.OutpDone
        )
        # "Vector iterator based on DesUtils.VectIter"
        VectIter2 : DesUtils.VectIter (
            _@ < LogLevel = "Dbg"
            InpV ~ SVect
            InpDone ~ : SB_True
            InpReset ~ : SB_False
        )
        VectIter2_Dbg : State (
            _@ < LogLevel = "Dbg"
            _@ < = "URI"
            Inp ~ VectIter2.OutV
        )
        VectIter2_Done_Dbg : State (
            _@ < LogLevel = "Dbg"
            Inp ~ VectIter2.OutpDone
        )
        {
            Vert1 : Elem
            Vert2 : Vert1
            Vert3 : Vert2
            Targets : Syst {
                CpExplorable : CpSystExplorable
                Target3 : Vert3
            }
            # "Controller uses adapter for access to target"
            Adapter : DAdp {
                LogLevel = "Dbg"
                Parents : SdoParents
            }
            Adapter  (
                CpExploring ~ Targets.CpExplorable
                InpMagUri ~ : Const {
                    = "URI Target3"
                }
            )
            # ">>> Example of AVR resolving type of CRP"
            CrpResolver : DesUtils.PrntMappingResolver2 (
                InpMpg ~ : State {
                    = "VPDU ( PDU ( URI Vert , URI VertCrp ) , PDU ( URI Vertu , URI VertCrp )  , PDU ( URI .MyRoot.Launcher.Vert2 , URI Vert2Crp )  , PDU ( URI Syst , URI SystCrp ) )"
                }
            )
            CrpResolver.InpParents ~ Adapter.Parents
            CrpResolver.InpDefRes ~ : Const {
                = "URI Vert"
            }
            CrpRes_Dbg : State (
                _@ < LogLevel = "Dbg"
                _@ < = "URI"
                Inp ~ CrpResolver.OutpRes
            )
            # "<<< Example of AVR resolving type of CRP"
        }
        {
            # ">>> Example: collecting of CPs of a systsem"
            CcpTargets : Syst {
                CpExplorable : CpSystExplorable
                CcpTarget : Syst {
                    Inp1 : ExtdStateInp
                    Outp1 : ExtdStateOutp
                }
            }
            # "Controller uses adapter for access to target"
            CcpAdapter : DAdp {
                LogLevel = "Dbg"
                CompsNames : SdoCompsUri
            }
            CcpAdapter  (
                CpExploring ~ CcpTargets.CpExplorable
                InpMagUri ~ TargUri : Const {
                    = "URI CcpTarget"
                }
            )
            CpsCollector : Des {
                # ">>> ConnPoints collector"
                # "INP: components names"
                InpCompNames : ExtdStateInp
                # "INP: Model link"
                CpObserving : ExtdSystExploring
                InpTargUri : ExtdStateInp
                CompsIter : DesUtils.VectIter (
                    _@ < LogLevel = "Dbg"
                    InpV ~ InpCompNames.Int
                    InpDone ~ : SB_True
                    InpReset ~ : SB_False
                )
                CompNames_Dbg : State (
                    _@ < LogLevel = "Dbg"
                    _@ < = "VDU"
                    Inp ~ InpCompNames.Int
                )
                CompsIter_Dbg : State (
                    _@ < LogLevel = "Dbg"
                    _@ < = "URI"
                    Inp ~ CompsIter.OutV
                )
                CompAdapter : DAdp (
                    CpExploring ~ CpObserving.Int
                    InpMagUri ~ : TrApndVar (
                        Inp1 ~ InpTargUri.Int
                        Inp2 ~ CompsIter.OutV
                    )
                    _@ <  {
                        LogLevel = "Dbg"
                        Parents : SdoParents
                        CpResolver : DesUtils.PrntMappingResolver (
                            InpParents ~ Parents
                            _@ <  {
                                Mapping < = "VPDU ( PDU ( URI ExtdStateInp , URI SysCrpInp ) , PDU ( URI ExtdStateOutp , URI SysCrpOutp ) )"
                                DefaultRes < = "URI SystCrpIo"
                            }
                        )
                    }
                )
                # "<<< ConnPoints collector"
            }
            CpsCollector.InpCompNames ~ CcpAdapter.CompsNames
            CpsCollector.CpObserving ~ CcpTargets.CpExplorable
            CpsCollector.InpTargUri ~ TargUri
            # "<<< Example: collecting of CPs of a systsem"
        }
    }
}
