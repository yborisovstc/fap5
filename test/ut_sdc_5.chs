Root : Elem {
    # "Test of SDC, ASdcConn connecting after target vertexes get created. Ref ISS_018"
    Launcher : DesLauncher {
        LogLevel = "Dbg"
        # "Model"
        Syst1 : Des {
            Syst1_1 : Des {
                Syst1_1_1 : Des
            }
            EnableCreate : State (
                _@ <  {
                    = "SB false"
                }
                Inp ~ : SB_True
            )
            SdcCreate : ASdcComp (
                _@ < LogLevel = "Dbg"
                Name ~ : Const {
                    = "SS Comp1"
                }
                Parent ~ : Const {
                    = "SS Vert"
                }
                Enable ~ EnableCreate
            )
            SdcCreateT : ASdcCompT (
                _@ < LogLevel = "Dbg"
                Target ~ : Const {
                    = "URI Syst1_1.Syst1_1_1"
                }
                Name ~ : Const {
                    = "SS Comp1"
                }
                Parent ~ : Const {
                    = "SS Vert"
                }
                Enable ~ EnableCreate
            )
        }
        Syst2 : Des {
            EnableCreate : State (
                _@ <  {
                    = "SB false"
                }
                Inp ~ : SB_True
            )
            SdcCreate : ASdcComp (
                _@ < LogLevel = "Dbg"
                Name ~ : Const {
                    = "SS Comp1"
                }
                Parent ~ : Const {
                    = "SS Vert"
                }
                Enable ~ EnableCreate
            )
        }
        # "Connect"
        Dc_Conn : ASdcConn (
            _@ < LogLevel = "Dbg"
            V1 ~ : State {
                = "SS Syst1.Comp1"
            }
            V2 ~ : State {
                = "SS Syst2.Comp1"
            }
            Enable ~ : SB_True
        )
        Conn_Dbg : State (
            _@ <  {
                LogLevel = "Dbg"
                = "SB false"
            }
            Inp ~ Dc_Conn.Outp
        )
    }
}
