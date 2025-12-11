MyRoot : Elem {
    Launcher : DesLauncher {
        About = "DES connecting, generic socket extending, ds_sock_extrd_uge"
        # "Doesnt improve main use-cases, denied"
        LogLevel = "Dbg"
        ESock : Socket2 {
            Pin1 : CpStateOutp
        }
        ESockP : Socket2 {
            Pin1 : CpStateInp
        }
        ESockExtd : ESock {
            Int : ESockP
        }
        ESockPExtd : ESockP {
            Int : ESock
        }
        Sock1 : Socket2 {
            Pin1 : CpStateOutp
            Pin2 : CpStateOutp
            Pin3 : ESock
        }
        Sock1p : Socket2 {
            Pin1 : CpStateInp
            Pin2 : CpStateInp
            Pin3 : ESockP
        }
        Sock1Extd : Sock1 {
            Int : Sock1p
        }
        Sock1pExtd : Sock1p {
            Int : Sock1
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
            ES1 ~ S1.Int.Pin3
            # "Test -pure- socket connection failed"
            ESock1 : ESock
            ESock1p : ESockP
            ESock1 ~ ESock1p
            S1E : Sock1Extd
            S1E.Int ~ S1
            ES2 : ESockExtd
            ES2.Int ~ S1E.Pin3
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
