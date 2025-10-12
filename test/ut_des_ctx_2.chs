Root : Elem {
    # "Test of DES context #2 Verifying context overlay"
    Launcher : DesLauncher {
        System1 : Des {
            # "Common context (stack)"
            CtxA : DesCtxSpl
            CtxB : DesCtxSpl
            St1 : State {
                LogLevel = "Dbg"
                = "SI 33"
            }
            CtxC : DesCtxSpl (
                _@ < Ctx1 : ExtdStateInp
                Ctx1 ~ St1
            )
            System2 : Des {
                # "Common context (stack)"
                CtxC : DesCtxCsm {
                    Ctx1 : ExtdStateOutp
                }
                Add1 : TrAddVar (
                    Inp ~ : State {
                        = "SI -1"
                    }
                )
                CtxC2 : DesCtxSpl (
                    _@ < Ctx1 : ExtdStateInp
                    Ctx1 ~ Add1
                )
                Add1.Inp ~ CtxC.Ctx1
                System3 : Des {
                    CtxD : DesCtxSpl {
                        CtxD1 : State {
                            LogLevel = "Dbg"
                            = "SI 44"
                        }
                    }
                    CtxC2 : DesCtxCsm {
                        Ctx1 : ExtdStateOutp
                    }
                    CtxC2_Dbg : State (
                        _@ <  {
                            LogLevel = "Dbg"
                            = "SI 0"
                        }
                        Inp ~ CtxC2.Ctx1
                    )
                }
            }
        }
    }
}
