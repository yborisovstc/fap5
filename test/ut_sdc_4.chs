Root : Elem {
    # "Test of SDC, connect and disconnect in cycle. Inp notif via Socket"
    Launcher : DesLauncher {
        LogLevel = "Dbg"
        # "Model"
        SConst_1 : Const {
            = "SI 1"
        }
        SConst_2 : Const {
            = "SI 2"
        }
        Sock1 : Socket {
            Data : CpStateInpPin
        }
        Sock1p : Socket {
            Data : CpStateOutpPin
        }
        SResult : Sock1 (
            _@ < LogLevel = "Dbg"
            Data ~ TrAdd2 : TrAddVar (
                Inp ~ : TrAddVar (
                    Inp ~ TrAdd : TrAddVar (
                        Inp ~ SConst_1
                    )
                )
                Inp ~ TrAdd
            )
        )
        Sock1p ~ SResult
        SResult_Dbg : State (
            _@ < LogLevel = "Dbg"
            _@ < = "SI"
            Inp ~ Sock1p.Data
        )
        # "Connect"
        Dc_Conn : ASdcConn (
            _@ < LogLevel = "Dbg"
            V1 ~ : Const {
                = "SS TrAdd2.Inp"
            }
            V2 ~ : Const {
                = "SS TrAdd"
            }
        )
        # "Disconnect"
        Dc_Disconn : ASdcDisconn (
            _@ < LogLevel = "Dbg"
            V1 ~ : Const {
                = "SS TrAdd2.Inp"
            }
            V2 ~ : Const {
                = "SS TrAdd"
            }
        )
        Dc_Conn.Enable ~ Dc_Disconn.Outp
        # "Form start condition"
        Dc_Disconn.Enable ~ : TrOrVar (
            Inp ~ : State (
                _@ < = "SB true"
                Inp ~ : State {
                    = "SB false"
                }
            )
            Inp ~ Dc_Conn.Outp
        )
    }
}
