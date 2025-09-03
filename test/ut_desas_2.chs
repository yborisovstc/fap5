MyRoot : Elem {
    # "DES active subsystem. Getting start of list"
    # "Test DESAS states initialization"
    Launcher : DesLauncher {
        LogLevel = "Dbg"
        ListElem : Syst {
            Prev : CpStateOutpPin
            Next : CpStateInpPin
        }
        ListStart : Syst {
            Prev : CpStateOutpPin
        }
        ListEnd : Syst {
            Next : CpStateInpPin
        }
        List : Syst {
            CpExplorable : CpSystExplorable
            Start : ListStart
            E1 : ListElem (
                Next ~ Start.Prev
            )
            E2 : ListElem (
                Next ~ E1.Prev
            )
            E3 : ListElem (
                Next ~ E2.Prev
            )
            E4 : ListElem (
                Next ~ E3.Prev
            )
            End : ListEnd (
                Next ~ E4.Prev
            )
        }
        ToListStart : DesAs2 {
            Subsys : DAdp {
                Init : State {
                    = "SB false"
                    LogLevel = "Dbg"
                }
                InitStateInp : ExtdStateInp
                Res : State {
                    = "URI"
                    LogLevel = "Dbg"
                }
                # "Managed node adapter"
                PairOfNext : SdoTcPair (
                    Targ ~ Res
                    TargComp ~ : Const {
                        = "URI Next"
                    }
                )
                PairOfNext_Dbg : State (
                    _@ <  {
                        LogLevel = "Dbg"
                        = "URI"
                    }
                    Inp ~ PairOfNext
                )
                PairOfNextOwner : SdoCompOwner (
                    Comp ~ PairOfNext
                )
                Res.Inp ~ : TrSwitchBool (
                    Inp1 ~ : TrSwitchBool (
                        Inp1 ~ Res
                        Inp2 ~ PairOfNextOwner
                        Sel ~ Cmp_Neq : TrCmpVar (
                            Inp ~ Res
                            Inp2 ~ : State {
                                = "URI .MyRoot.Launcher.List.Start"
                            }
                        )
                    )
                    Inp2 ~ InitStateInp.Int
                    Sel ~ Init
                )
            }
            AsRes : State (
                _@ <  {
                    LogLevel = "Dbg"
                    = "URI"
                }
                Inp ~ Subsys.Res
            )
        }
        Init : State (
            _@ <  {
                LogLevel = "Dbg"
                = "URI E2"
            }
            Inp ~ Sw1 : TrSwitchBool (
                Inp2 ~ : State {
                    = "URI E3"
                }
                Sel ~ Cmp_Neq : TrCmpVar (
                    Inp ~ Init
                    Inp2 ~ : State {
                        = "URI E3"
                    }
                )
            )
        )
        Sw1.Inp1 ~ Init
        ToListStart.Subsys.InitStateInp ~ Init
        ToListStart.Subsys.CpExploring ~ List.CpExplorable
        ToListStart.Subsys.InpMagUri ~ : Const {
            = "SS"
        }
    }
}
