MyRoot : Elem {
    Launcher : DesLauncher {
        About = "Simple DES test, invalidating on conn/disconn"
        LogLevel = "Dbg"
        Ds1 : Des {
            Const_1 : State {
                = "SI 1"
            }
            St1 : State (
                _@ <  {
                    = "SI 0"
                    LogLevel = "Dbg"
                }
                Inp ~ Add : TrAddVar (
                    Inp ~ St1
                    Inp ~ Const_1
                )
            )
        }
    }
}
