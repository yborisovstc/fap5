MyRoot : Elem {
    Launcher : DesLauncher {
        About = "DES connecting, emb sockets extd"
        LogLevel = "Dbg"
        ESock : Socket {
            Pin1 : ExtdStateInp
        }
        ESockP : Socket {
            Pin1 : ExtdStateOutp
        }
        ESockPin : SocketExtdInt {
            Pin1 : CpStateInp
        }
        ESockPinP : SocketExtdInt {
            Pin1 : CpStateOutp
        }
        ESockExtdInt : SocketExtdInt {
            Pin1 : CpStateOutp
        }
        ESockExtd : SocketExtd {
            Pin1 : CpStateInp
            Int : ESockExtdInt
        }
        ESockPExtdInt : SocketExtdInt {
            Pin1 : CpStateInp
        }
        ESockPExtd : SocketExtd {
            Pin1 : CpStateOutp
            Int : ESockPExtdInt
        }
        Sock1 : Socket {
            Pin1 : ExtdStateInp
            Pin2 : ExtdStateInp
            Pin3 : ESockExtd
        }
        Sock1p : Socket {
            Pin1 : ExtdStateOutp
            Pin2 : ExtdStateOutp
            Pin3 : ESockPExtd
        }
        Sock1ExtdInt : SocketExtdInt {
            Pin1 : CpStateInp
            Pin2 : CpStateInp
            Pin3 : ESockPin
        }
        Sock1Extd : SocketExtd {
            Pin1 : CpStateOutp
            Pin2 : CpStateOutp
            Pin3 : ESockPinP
            Int : Sock1ExtdInt
        }
        Ds1 : Des {
            S1 : Sock1 (
                Pin1 ~ : SI_0
                Pin2 ~ : SI_1
            )
            ES1 : ESock (
                Pin1 ~ : Const {
                    = "SI 7"
                }
            )
            ES1 ~ S1.Pin3
            S1E : Sock1Extd
            S1E.Int ~ S1
            S2 : Sock1p
            S2 ~ S1E
            ESP : ESockP
            ESP ~ S2.Pin3
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
            St3 : State (
                _@ <  {
                    = "SI"
                    LogLevel = "Dbg"
                }
                Inp ~ ESP.Pin1
            )
        }
    }
}
