MyRoot : Elem {
    Launcher : DesLauncher {
        About = "DES connecting, generic socket extending, ds_sock_extrd_uge"
        # "Doesnt improve main use-cases, denied"
        LogLevel = "Dbg"
        ESock : Socket3 {
            Pin1 : CpStateOutp
            BPeer = "ESockP"
        }
        ESockP : Socket3 {
            Pin1 : CpStateInp
        }
        ESockExtd : Socket3Extd {
            Pin1 : CpStateOutp
        }
        Sock1Extd : Socket3Extd {
            Pin1 : CpStateOutp
            Pin2 : CpStateOutp
            Pin3 : ESock
            Pin4 : Socket3 {
                Pin1 : CpStateOutp
            }
        }
        Ds1 : Des {
            S1 : Sock1Extd (
                Int.Pin1 ~ : SI_0
                Int.Pin2 ~ : SI_1
            )
            ES1 : ESockExtd (
                Int.Pin1 ~ : Const {
                    = "SI 7"
                }
            )
            ES1 ~ S1.Int.Pin4
            S1E : Sock1Extd
            S1E.Int ~ S1
            ES2 : ESockExtd
            ES2.Int ~ S1E.Pin4
            St1 : State (
                _@ <  {
                    = "SI"
                    LogLevel = "Dbg"
                }
                Inp ~ S1E.Pin1
            )
            St2 : State (
                _@ <  {
                    = "SI"
                    LogLevel = "Dbg"
                }
                Inp ~ S1E.Pin2
            )
            St3 : State (
                _@ <  {
                    = "SI"
                    LogLevel = "Dbg"
                }
                Inp ~ ES2.Pin1
            )
        }
    }
}
