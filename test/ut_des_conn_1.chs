MyRoot : Elem {
    Launcher : DesLauncher {
        About = "DES connecting"
        LogLevel = "Dbg"
        ESock : Socket {
            Pin1 : ExtdStateInp
        }
        ESockP : Socket {
            Pin1 : ExtdStateOutp
        }
        Sock1 : Socket {
            Pin1 : ExtdStateInp
            Pin2 : ExtdStateInp
            Pin3 : ESock
        }
        Sock1p : Socket {
            Pin1 : ExtdStateOutp
            Pin2 : ExtdStateOutp
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
            S3 : Sock1p
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
            ExtSockPin : SocketExtdInt {
                Pin1 : CpStateOutp
            }
            ExtSockPinP : SocketExtdInt  {
                Pin1 : CpStateInp
            }
            SockExtdIntCp : SocketExtdInt {
                Pin1 : CpStateInp
                Pin2 : CpStateInp
                Pin3 : ExtSockPinP
            }
            SockExtd : SocketExtd {
                Pin1 : CpStateOutp
                Pin2 : CpStateOutp
                Pin3 : ExtSockPin
                Int : SockExtdIntCp
            }
            SockExtd2 : SocketExtd {
                Pin1 : CpStateOutp
                Pin2 : CpStateOutp
                Pin3 : ExtSockPin
                Int : SockExtdIntCp
            }

            S3 ~ SockExtd2

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
        Ds2 : Des {
            Sock1 : Socket {
                Pin1 : ExtdStateInp
                Pin2 : ExtdStateInp
            }
            Sock1p : Socket {
                Pin1 : ExtdStateOutp
                Pin2 : ExtdStateOutp
            }
            S1 : Sock1 (
                Pin1 ~ : SI_0
                Pin2 ~ : SI_1
            )
            S1p : Sock1p
            S1p ~ S1
            St2_1 : State (
                _@ <  {
                    = "SI"
                    LogLevel = "Dbg"
                }
                Inp ~ S1p.Pin1
            )
        }
    }
}
