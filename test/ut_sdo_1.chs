Root : Elem {
    # "Test of SDO"
    Launcher : DesLauncher {
        # "Components to be checked"
        V1 : Vert
        V2 : Vert
        V3 : Vert
        V4 : Vert {
            V4_1 : Vert
        }
        V5 : Vert
        V6 : Vert
        V1 ~ V2
        V1 ~ V4
        V5 ~ V4
        V5 ~ V3
        V4.V4_1 ~ V6
        # "Checking existence of component"
        Dbg_DcoComp : State (
            _@ <  {
                LogLevel = "Dbg"
                = "SB false"
            }
            Inp ~ DcoComp : SdoComp (
                _@ < LogLevel = "Dbg"
                Name ~ : Const {
                    = "URI V1"
                }
            )
        )
        # "Checking connection"
        Dbg_DcoConn : State (
            _@ <  {
                LogLevel = "Dbg"
                = "SB false"
            }
            Inp ~ DcoConn : SdoConn (
                _@ < LogLevel = "Dbg"
                Vp ~ : Const {
                    = "URI V1"
                }
                Vq ~ : Const {
                    = "URI V2"
                }
            )
        )
        # "Checking connection V1 ~ V3"
        Dbg_DcoConn2 : State (
            _@ <  {
                LogLevel = "Dbg"
                = "SB false"
            }
            Inp ~ DcoConn2 : SdoConn (
                _@ < LogLevel = "Dbg"
                Vp ~ : Const {
                    = "URI V1"
                }
                Vq ~ : Const {
                    = "URI V3"
                }
            )
        )
        # "Pairs count"
        Dbg_DcoPairsCount : State (
            _@ <  {
                LogLevel = "Dbg"
                = "SI 0"
            }
            Inp ~ DcoPairsCount : SdoPairsCount (
                _@ < LogLevel = "Dbg"
                Vp ~ : Const {
                    = "URI V1"
                }
            )
        )
        # "Target Pair"
        Dbg_DcoPair : State (
            _@ <  {
                LogLevel = "Dbg"
                = "URI"
            }
            Inp ~ DcoPair : SdoPair (
                _@ < LogLevel = "Dbg"
                Vp ~ : Const {
                    = "URI V2"
                }
            )
        )
        # "Component owner"
        Dbg_DcoCompOwner : State (
            _@ <  {
                LogLevel = "Dbg"
                = "URI"
            }
            Inp ~ DcoCompOwner : SdoCompOwner (
                _@ < LogLevel = "Dbg"
                Comp ~ : State {
                    = "URI V4.V4_1"
                }
            )
        )
        # "Component comp"
        Dbg_DcoCompComp : State (
            _@ <  {
                LogLevel = "Dbg"
                = "URI"
            }
            Inp ~ DcoCompComp : SdoCompComp (
                _@ < LogLevel = "Dbg"
                Comp ~ : State {
                    = "URI V4"
                }
                CompComp ~ : State {
                    = "URI V4_1"
                }
            )
        )
        # "V5 pairs"
        Dbg_DcoPairs : State (
            _@ <  {
                LogLevel = "Dbg"
                = "VU _INV"
            }
            Inp ~ V5.DcoPairs
        )
        # "Component V5 pairs"
        Dbg_DcoTPairs : State (
            _@ <  {
                LogLevel = "Dbg"
                = "VU _INV"
            }
            Inp ~ DcoTPairs : SdoTPairs (
                _@ < LogLevel = "Dbg"
                Targ ~ : State {
                    = "URI V5"
                }
            )
        )
        # "Component of target pair"
        Dbg_DcoTcPair : State (
            _@ <  {
                LogLevel = "Dbg"
                = "URI"
            }
            Inp ~ DcoTcPair : SdoTcPair (
                _@ < LogLevel = "Dbg"
                Targ ~ : State {
                    = "URI V4"
                }
                TargComp ~ : State {
                    = "URI V4_1"
                }
            )
        )
    }
}
