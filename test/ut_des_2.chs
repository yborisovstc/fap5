MyRoot : Elem {
    Launcher : DesLauncher {
        About = "Simple DES test, invalidating on conn/disconn"
        LogLevel = "Dbg"
        Ds1 : Des {
            LogLevel = "Dbg"
            Const_1 : State {
                LogLevel = "Dbg"
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
        Ds2 : Ds1
        Ds3 : Ds1
        Ds4 : Ds1
        Ds5 : Ds1
        Ds6 : Ds1
        Ds7 : Ds1
        Ds8 : Ds1
    }
}
