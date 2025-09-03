MyRoot : Elem {
    # "DES active subsystem. Test #1"
    # "TODO DesAs is used, so not working. To remove"
    Launcher : DesLauncher {
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
            As1 : DesAs {
                AsInp : ExtdStateInp
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
                                Inp2 ~ AsInp.Int
                            )
                        )
                        Inp2 ~ : SI_0
                        Sel ~ Init
                    )
                )
            }
            As1.AsInp ~ St1
            As1Res : State {
                = "SI"
                LogLevel = "Dbg"
            }
            As1Res.Inp ~ As1.St
        }
    }
}
