MyRoot : Elem {
    Launcher : DesLauncher {
        About = "DES connecting"
        LogLevel = "Dbg"
        Sock1 : Socket {
            Pin1 : CpStateInpPin
            Pin2 : CpStateInpPin
        }
        Sock1p : Socket {
            Pin1 : CpStateOutpPin
            Pin2 : CpStateOutpPin
        }
        Ds1 : Des {
            S1 : Sock1 (
                Pin1 ~ : SI_0
                Pin2 ~ : SI_1
            )
            S2 : Sock1p
            St1 : State (
                _@ <  {
                    = "SI"
                    LogLevel = "Dbg"
                }
                Inp ~ S2.Pin1
            )
            St2 : State (
                _@ <  {
                    = "SI"
                    LogLevel = "Dbg"
                }
                Inp ~ S2.Pin2
            )
            SockExtd : SocketExtd {
                Pin1 : CpStateInpPin
                Pin2 : CpStateInpPin
                Int : Sock1p
            }
            SockExtd.Int ~ S1
            S2 ~ SockExtd
        }
    }
}
