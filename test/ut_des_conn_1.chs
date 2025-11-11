MyRoot : Elem {
    Launcher : DesLauncher {
        About = "DES connecting"
        LogLevel = "Dbg"
        ESock : Socket {
            Pin1 : CpStateInp
        }
        ESockP : Socket {
            Pin1 : CpStateOutp
        }
        Sock1 : Socket {
            Pin1 : CpStateInp
            Pin2 : CpStateInp
            Pin3 : ESock
        }
        Sock1p : Socket {
            Pin1 : CpStateOutp
            Pin2 : CpStateOutp
            Pin3 : ESockP
        }
        Ds1 : Des {
            S1 : Sock1 (
                Pin1 ~ : SI_0
                Pin2 ~ : SI_1
                Pin3.Pin1 ~ : Const {
                    = "SI 7"
                }
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
                Pin1 : CpStateInp
                Pin2 : CpStateInp
                Pin3 : ESock
                Int : Sock1p
            }
            SockExtd.Int ~ S1
            S2 ~ SockExtd
            St3 : State (
                _@ <  {
                    = "SI"
                    LogLevel = "Dbg"
                }
                Inp ~ S2.Pin3.Pin1
            )
        }
    }
}
