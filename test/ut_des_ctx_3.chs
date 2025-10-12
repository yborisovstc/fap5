Root : Elem {
    # "Test of DES context #2, Multiple consumer"
    Launcher : DesLauncher {
        System1 : Des {
            # "Common context (stack)"
            CtxA : DesCtxSpl
            CtxB : DesCtxSpl
            St1 : State {
                LogLevel = "Dbg"
                = "SI 33"
            }
            St2 : State {
                LogLevel = "Dbg"
                = "SS St2 data"
            }
            CtxC : DesCtxSpl (
                _@ < Ctx1 : ExtdStateInp
                Ctx1 ~ St1
                _@ < Ctx2 : ExtdStateInp
                Ctx2 ~ St2
            )
            System2 : Des {
                # "Common context (stack)"
                CtxC : DesCtxCsm {
                    Ctx1 : ExtdStateOutp
                    Ctx2 : ExtdStateOutp
                }
                Ctx1add : TrAddVar (
                    Inp ~ : State {
                        = "SI -1"
                    }
                    Inp ~ CtxC.Ctx1
                )
                CtxC_Ctx1_add_Dbg : State (
                    _@ <  {
                        LogLevel = "Dbg"
                        = "SI 0"
                    }
                    Inp ~ Ctx1add
                )
                CtxC_Ctx2_Dbg : State (
                    _@ <  {
                        LogLevel = "Dbg"
                        = "SS"
                    }
                    Inp ~ CtxC.Ctx2
                )
            }
            System3 : Des {
                CtxC : DesCtxCsm {
                    Ctx1 : ExtdStateOutp
                    Ctx2 : ExtdStateOutp
                }
                CtxC_Ctx1_Dbg : State (
                    _@ <  {
                        LogLevel = "Dbg"
                        = "SI 0"
                    }
                    Inp ~ CtxC.Ctx1
                )
                CtxC_Ctx2_Dbg : State (
                    _@ <  {
                        LogLevel = "Dbg"
                        = "SS"
                    }
                    Inp ~ CtxC.Ctx2
                )
            }
        }
    }
}
