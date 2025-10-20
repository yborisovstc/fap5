MyRoot : Elem {
    Launcher : DesLauncher {
        About = "DES connecting"
        LogLevel = "Dbg"
        Sock1 : Socket {
            Inp1 : CpStateInpPin
            Inp2 : CpStateInpPin
        }
        Ds1 : Des {
            S1 : Sock1 (
                Inp1 ~ : SI_0
                Inp2 ~ : SI_1
            )
            S2 : Sock1
            St1 : State (
                _@ <  {
                    = "SI"
                    LogLevel = "Dbg"
                }
                Inp ~ S2.Inp1
            )
            St2 : State (
                _@ <  {
                    = "SI"
                    LogLevel = "Dbg"
                }
                Inp ~ S2.Inp2
            )
            SockExtd : Extd {
                Int : Sock1
            }
            S2 ~ SockExtd
            SockExtd.Int.Inp1 ~ : SI_0
            SockExtd.Int.Inp2 ~ : SI_2
        }
    }
}
