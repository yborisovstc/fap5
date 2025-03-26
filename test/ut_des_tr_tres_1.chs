MyRoot : Elem {
    Launcher : DesLauncher {
        S1 : State (
            _@ <  {
                = "SB"
                LogLevel = "Dbg"
            }
            Inp ~ Cmp_Gt : TrCmpVar (
                Inp ~ : TrAddVar (
                    Inp ~ : State {
                        = "SI 2"
                    }
                    InpN ~ : State {
                        = "SI 1"
                    }
                )
                Inp2 ~ : State {
                    = "SI 3"
                }
                _@ < LogLevel = "Err"
            )
        )
    }
}
