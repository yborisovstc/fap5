Root : Elem {
    # "Test of DES context #1"
    # "Verifying that DES resolves suppliers stack head only, ref ds_dctx_dic_cs"
    Launcher : DesLauncher {
        System1 : Des {
            # "Common context (stack)"
            CtxA : DesCtxSpl
            CtxB : DesCtxSpl
            CtxC : DesCtxSpl
            System2 : Des {
                # "Common context (stack)"
                CtxC : DesCtxSpl {
                    Ctx1 : ExtdStateInp
                }
                Count1 : State (
                    _@ < = "SI 0"
                    Inp ~ : TrAddVar (
                        Inp ~ Count1
                        Inp ~ : SI_1
                    )
                )
                CtxC.Ctx1 ~ Count1
                System3 : Des {
                    CtxD : DesCtxSpl {
                        CtxD1 : State {
                            LogLevel = "Dbg"
                            = "SI 44"
                        }
                    }
                    CtxC : DesCtxCsm {
                        Ctx1 : ExtdStateOutp
                    }
                    CtxC_Dbg : State (
                        _@ <  {
                            LogLevel = "Dbg"
                            = "SI 0"
                        }
                        Inp ~ CtxC.Ctx1
                    )
                }
            }
        }
    }
}
