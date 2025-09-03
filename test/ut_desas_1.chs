MyRoot : Elem {
    # "DES active subsystem. Test #1"
    Launcher : DesLauncher {
        LogLevel = "Dbg"
        Ds1 : Des {
            St1 : State {
                = "SI 5"
                LogLevel = "Dbg"
            }
            St1.Inp ~ : TrAddVar (
                Inp ~ St1
                Inp ~ : State {
                    = "SI 4"
                }
            )
            StDone : State {
                = "SB"
                LogLevel = "Dbg"
            }
            As1 : DesAs2 {
                AsInp : ExtdStateInp
                Subsys : Des {
                    SsInp : ExtdStateInp
                    Init : State {
                        = "SB false"
                    }
                    St : State (
                        _@ <  {
                            = "SI 0"
                            LogLevel = "Dbg"
                        }
                        Inp ~ : TrSwitchBool (
                            Inp1 ~ Sw1 : TrSwitchBool (
                                Inp1 ~ St
                                Inp2 ~ : TrAddVar (
                                    Inp ~ St
                                    Inp ~ : State {
                                        = "SI 1"
                                    }
                                )
                                Sel ~ Cmp_Neq : TrCmpVar (
                                    Inp ~ St
                                    Inp2 ~ SsInp.Int
                                )
                            )
                            Inp2 ~ : SI_0
                            Sel ~ Init
                        )
                    )
                }
                Subsys.SsInp ~ AsInp.Int
                Res : State {
                    = "SI"
                    LogLevel = "Dbg"
                }
                Res.Inp ~ Subsys.St
            }
            As1.AsInp ~ St1
            AsRes : State {
                = "SI"
                LogLevel = "Dbg"
            }
            AsRes.Inp ~ As1.Res
        }
    }
}
